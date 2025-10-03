interface Config {
    meta: {
        version: string,
    }
}

const filePath = "./mod.json";
const modConfig: Config = JSON.parse(await Deno.readTextFile(filePath));

// Output the new version for GitHub Actions
console.log(modConfig.meta.version);
