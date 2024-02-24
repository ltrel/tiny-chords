import { defineConfig } from 'tsup'

export default defineConfig({
  entry: ['src/wasm.ts', 'src/nowasm.ts'],
  sourcemap: false,
  minify: true,
  clean: true,
  format: ['cjs', 'esm'],
  dts: true,
})
