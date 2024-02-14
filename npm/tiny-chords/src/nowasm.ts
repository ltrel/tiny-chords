import TinyChords from "../cppbuild/tinychords-nowasm";
import { MainModule } from "../cppbuild/interface"
import { generateBinToJson, generateJsonToBin} from "./common";

const noWasmModPromise: Promise<MainModule> = TinyChords();
export const binToJson = generateBinToJson(noWasmModPromise);
export const jsonToBin = generateJsonToBin(noWasmModPromise);
