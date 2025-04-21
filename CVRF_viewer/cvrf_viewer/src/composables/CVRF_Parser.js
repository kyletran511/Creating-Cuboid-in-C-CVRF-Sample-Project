import { ref, computed, onMounted, watch } from 'vue';
import { XMLParser } from 'fast-xml-parser';

export function useCvrfData(itemsPerPage = 10) {
  const vulnerabilities = ref([]);
  const searchQuery = ref('');
  const sortKey = ref('id'); // Default sorting by CVE ID
  const sortAsc = ref(true);
  const currentPage = ref(1);

  const parseCvrfXML = (xmlString) => {
    const parser = new XMLParser({
      ignoreAttributes: false,
      attributeNamePrefix: '',
      removeNSPrefix: true,
      trimValues: true,
    });

    const json = parser.parse(xmlString);
    const root = json.cvrfdoc;

    if (!root || !root.Vulnerability) return [];

    const rawVulns = Array.isArray(root.Vulnerability)
      ? root.Vulnerability
      : [root.Vulnerability];

    return rawVulns
      .map((vuln) => {
        const title = vuln.Title || 'No Title';
        const cve = vuln.CVE || 'No CVE';

        let description = 'No description';
        let published = 'N/A';
        let modified = 'N/A';

        const notes = Array.isArray(vuln.Notes?.Note)
          ? vuln.Notes.Note
          : vuln.Notes?.Note
          ? [vuln.Notes.Note]
          : [];

        for (const note of notes) {
          if (note.Type === 'Description' && typeof note === 'object') {
            const text = note['#text'] || '';
            if (text.includes('** RESERVED **')) return null;
            description = text;
          } else if (note.Title === 'Published') {
            published = note['#text'] || published;
          } else if (note.Title === 'Modified') {
            modified = note['#text'] || modified;
          }
        }

        const refList = vuln.References?.Reference;
        const references = Array.isArray(refList)
          ? refList
          : refList
          ? [refList]
          : [];

        const parsedRefs = references.map((ref) => ({
          url: ref.URL || '',
          description: ref.Description || '',
        }));

        return {
          id: cve,
          title,
          description,
          published,
          modified,
          references: parsedRefs,
        };
      })
      .filter(Boolean);
  };

  const loadXML = async () => {
    try {
      const xml = await fetch('/allitems-cvrf-year-2022.xml').then((res) =>
        res.text()
      );
      vulnerabilities.value = parseCvrfXML(xml);
    } catch (err) {
      console.error('Error loading/parsing CVRF XML:', err);
    }
  };

  const filtered = computed(() =>
    vulnerabilities.value.filter((v) =>
      v.description.toLowerCase().includes(searchQuery.value.toLowerCase())
    )
  );

  const sorted = computed(() => {
    return [...filtered.value].sort((a, b) => {
      const aVal = a[sortKey.value];
      const bVal = b[sortKey.value];

      if (!aVal || !bVal) return 0;

      // Try to sort by date if possible
      const isDate = sortKey.value === 'published' || sortKey.value === 'modified';
      const valA = isDate ? new Date(aVal) : aVal.toString().toLowerCase();
      const valB = isDate ? new Date(bVal) : bVal.toString().toLowerCase();

      if (valA < valB) return sortAsc.value ? -1 : 1;
      if (valA > valB) return sortAsc.value ? 1 : -1;
      return 0;
    });
  });

  const totalPages = computed(() =>
    Math.ceil(sorted.value.length / itemsPerPage)
  );

  const paginated = computed(() => {
    const start = (currentPage.value - 1) * itemsPerPage;
    return sorted.value.slice(start, start + itemsPerPage);
  });

  const sortBy = (key) => {
    if (sortKey.value === key) {
      sortAsc.value = !sortAsc.value;
    } else {
      sortKey.value = key;
      sortAsc.value = true;
    }
  };

  const goToPage = (page) => {
    if (page >= 1 && page <= totalPages.value) currentPage.value = page;
  };

  const nextPage = () => {
    if (currentPage.value < totalPages.value) currentPage.value++;
  };

  const prevPage = () => {
    if (currentPage.value > 1) currentPage.value--;
  };

  watch(searchQuery, () => {
    currentPage.value = 1;
  });

  onMounted(loadXML);

  return {
    paginated,
    searchQuery,
    sortBy,
    sortKey,
    sortAsc,
    currentPage,
    totalPages,
    nextPage,
    prevPage,
    goToPage,
  };
}
