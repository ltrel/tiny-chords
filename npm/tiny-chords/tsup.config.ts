import { defineConfig } from 'tsup'

export default defineConfig({
  entry: ['src/wasm.ts', 'src/nowasm.ts'],
  splitting: false,
  sourcemap: true,
  clean: true,
  format: ['cjs', 'esm'],
  dts: true,
})
