import TinyChords from "../cppbuild/tinychords";
import { MainModule } from "../cppbuild/interface"
import { generateBinToJson, generateJsonToBin} from "./common";

const wasmModPromise: Promise<MainModule> = TinyChords();
export const binToJson = generateBinToJson(wasmModPromise);
export const jsonToBin = generateJsonToBin(wasmModPromise);
