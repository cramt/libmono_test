const fs = require("fs")
const path = require("path")
const copyFile = (from, to) => {
    fs.createReadStream(from).pipe(fs.createWriteStream(to))
}

const packagesFolder = "packages"

if (fs.lstatSync(packagesFolder).isDirectory) {
    fs.readdirSync(packagesFolder).forEach(x => {
        const subDir = path.resolve(packagesFolder, x)
        if (fs.lstatSync(subDir).isDirectory && fs.lstatSync(path.resolve(subDir, "lib")).isDirectory) {
            const libFolder = path.resolve(packagesFolder, x, "lib")
            fs.readdirSync(libFolder).forEach(versionName => {
                const versionFolder = path.resolve(libFolder, versionName)
                if (fs.lstatSync(versionFolder).isDirectory) {
                    fs.readdirSync(versionFolder).forEach(dllName => {

                        const temp = dllName.split(".")
                        if (temp[temp.length - 1] == "dll") {
                            const dllPath = path.resolve(versionFolder, dllName)
                            const outPath = path.resolve("x64", "Debug", "dll_dep")
                            if (!fs.existsSync(outPath)) {
                                fs.mkdirSync(outPath);
                            }
                            copyFile(dllPath, path.resolve(outPath, dllName))
                        }

                    })

                }
            })
        }
    })
}