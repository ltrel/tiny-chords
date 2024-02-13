import TinyChords from "../cppbuild/tinychords";
import { MainModule } from "../cppbuild/interface"

const modPromise: Promise<MainModule> = TinyChords();
export async function jsonToBin(jsonStr) {
    const mod = await modPromise;
    return mod.jsonToBin(jsonStr)
}
