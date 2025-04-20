<template>
    <h2 class="text-3xl font-bold text-red-600">Tailwind is working!</h2>

    <div>
      <!-- Search -->
      <input
        v-model="searchQuery"
        type="text"
        placeholder="Search vulnerabilities by description..."
        class="w-full mb-6 px-4 py-2 border border-gray-300 rounded shadow-sm focus:ring-2 focus:ring-blue-300"
      />
  
      <!-- Table -->
      <div class="overflow-x-auto border rounded-lg shadow-md bg-white">
        <table class="w-full border-collapse text-sm">
          <thead class="bg-gray-100 text-left">
            <tr>
              <th @click="sortBy('title')" class="px-4 py-3 border cursor-pointer">Title</th>
              <th class="px-4 py-3 border">Description</th>
              <th @click="sortBy('published')" class="px-4 py-3 border cursor-pointer">Published</th>
              <th class="px-4 py-3 border">Modified</th>
              <th class="px-4 py-3 border">References</th>
            </tr>
          </thead>
          <tbody>
            <tr
              v-for="vuln in paginated"
              :key="vuln.id"
              class="hover:bg-blue-50 border-t"
            >
              <td class="px-4 py-3 border font-medium break-all prose max-w-none">{{ vuln.title }}</td>
              <td class="px-4 py-3 border whitespace-pre-wrap prose max-w-none">{{ vuln.description }}</td>
              <td class="px-4 py-3 border prose max-w-none">{{ vuln.published }}</td>
              <td class="px-4 py-3 border prose max-w-none">{{ vuln.modified }}</td>
              <td class="px-4 py-3 border prose max-w-none">
                <ul v-if="vuln.references.length > 0" class="space-y-1">
                  <li
                    v-for="(ref, i) in vuln.references"
                    :key="i"
                  >
                    <a
                      :href="ref.url"
                      target="_blank"
                      class="text-blue-600 underline hover:text-blue-800 transition"
                    >
                      {{ ref.description || ref.url }}
                    </a>
                  </li>
                </ul>
                <span v-else class="text-gray-400 italic">No references</span>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
  
      <!-- Pagination -->
      <div class="mt-6 flex justify-center items-center gap-2 flex-wrap">
        <button
          @click="goToPage(1)"
          :disabled="currentPage === 1"
          class="px-3 py-1 border rounded disabled:opacity-50"
        >
          « First
        </button>
  
        <button
          @click="prevPage"
          :disabled="currentPage === 1"
          class="px-3 py-1 border rounded disabled:opacity-50"
        >
          ‹ Prev
        </button>
  
        <template v-for="page in visiblePages" :key="page">
          <button
            v-if="page !== '...'"
            @click="goToPage(page)"
            :class="[
              'px-3 py-1 border rounded',
              currentPage === page ? 'bg-blue-600 text-white' : 'bg-white'
            ]"
          >
            {{ page }}
          </button>
          <span v-else class="px-2">...</span>
        </template>
  
        <button
          @click="nextPage"
          :disabled="currentPage === totalPages"
          class="px-3 py-1 border rounded disabled:opacity-50"
        >
          Next ›
        </button>
  
        <button
          @click="goToPage(totalPages)"
          :disabled="currentPage === totalPages"
          class="px-3 py-1 border rounded disabled:opacity-50"
        >
          Last »
        </button>
      </div>
    </div>
  </template>
  
  <script setup>
  import { computed } from 'vue';
  import { useCvrfData } from '../composables/CVRF_Parser';
  
  const {
    paginated,
    searchQuery,
    sortBy,
    currentPage,
    totalPages,
    nextPage,
    prevPage,
    goToPage,
  } = useCvrfData(10);
  
  // Limit page buttons to a max of 5 with "..."
  const visiblePages = computed(() => {
    const pages = [];
    const total = totalPages.value;
    const current = currentPage.value;
  
    if (total <= 5) {
      for (let i = 1; i <= total; i++) pages.push(i);
    } else {
      if (current > 2) pages.push(1);
      if (current > 3) pages.push('...');
  
      const start = Math.max(2, current - 1);
      const end = Math.min(total - 1, current + 1);
  
      for (let i = start; i <= end; i++) pages.push(i);
  
      if (current < total - 2) pages.push('...');
      if (current < total - 1) pages.push(total);
    }
  
    return pages;
  });
  </script>
  
  