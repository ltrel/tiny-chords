import { MainModule } from "../cppbuild/interface"

export interface TinyChordChart {
    beatsPerBar: number;
    beatType: 'half' | 'quarter' | 'eighth' | 'sixteenth';
    bpm: number;
    chords: Array<{
        root: string;
        chordType: string;
        beats: number;
        bass?: string;
    }>
}

export function generateJsonToBin(modPromise: Promise<MainModule>) {
    async function jsonToBin(jsonStr: string) {
        const mod = await modPromise;
        const vec = mod.jsonToBin(jsonStr);
        const arr = new Uint8Array(vec.size()).fill(0).map((_, i) => vec.get(i));
        return arr;
    }
    return jsonToBin;
}

export function generateBinToJson(modPromise: Promise<MainModule>) {
    async function binToJson(binArray: Uint8Array): Promise<TinyChordChart> {
        const mod = await modPromise;
        const vec = new mod.uint8vec();
        binArray.forEach((byte) => vec.push_back(byte));
        const jsonStr = mod.binToJson(vec);
        return JSON.parse(jsonStr);
    }
    return binToJson;
}
