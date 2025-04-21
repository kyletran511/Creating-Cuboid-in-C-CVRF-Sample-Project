<template>
  <main class="min-h-screen bg-gradient-to-b from-gray-50 to-gray-100 text-gray-800 font-sans">
    <header class="bg-blue-700 text-white px-6 py-4 shadow">
      <h1 class="text-2xl font-bold">CVRF Vulnerability Viewer</h1>
    </header>
    <section class="max-w-7xl mx-auto p-6">
      <CvrfTable />
    </section>
  </main>
</template>

<script setup>
import CvrfTable from './components/CVRF_Table.vue';
</script>