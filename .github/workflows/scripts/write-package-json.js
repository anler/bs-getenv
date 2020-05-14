const fs = require("fs");
const path = require("path");

const packageDesc = require("../../../package.json");

const packageJson = JSON.stringify(
    {
        ...packageDesc,
        scripts: {
            postinstall: "node ./postinstall.js"
        }
    },
    null,
    2
);

fs.writeFileSync(
    path.join(__dirname, "..", "..", "..", "_release", "package.json"),
    packageJson,
    { encoding: "utf8" }
);
