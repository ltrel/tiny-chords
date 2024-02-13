import TinyChords from "../cppbuild/tinychords-nowasm";
import { MainModule } from "../cppbuild/interface"

const modPromise: Promise<MainModule> = TinyChords();
export async function jsonToBin(jsonStr) {
    const mod = await modPromise;
    return mod.jsonToBin(jsonStr)
}
