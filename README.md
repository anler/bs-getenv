# bs-getenv

![bs-getenv pipeline](https://github.com/anler/bs-getenv/workflows/bs-getenv%20pipeline/badge.svg) [![npm version](https://img.shields.io/npm/v/bs-getenv.svg)](https://www.npmjs.com/package/bs-getenv)

PPX for [ReasonML](https://reasonml.github.io) / [BuckleScript](https://bucklescript.github.io) for embedding
environment variables in the code.

![bs-getenv](./.assets/example.png)

## Installation
Get the package:

```shell
# yarn
yarn add bs-getenv
# or npm
npm install --save bs-getenv
```

Then add it to `bsconfig.json`:

```json
"ppx-flags": ["bs-getenv/ppx"]
```

## Usage

There are two patterns:

### With a default value

Usage: `[%getenv "VAR_NAME"; defaultValue]`, where:

- The result type will be `string`
- `defaultValue` can be any expression of type `string`

### Without a default value

Usage: `[%getenv "VAR_NAME"]`, where:

- The result type will be `option(string)`.

## Example
```reason
let getDefaultValue = () => "Some other value";
let var1: string = [%getenv "SOME_VAR_THAT_DOESNT_EXIST"; getDefaultValue()];

let var2: string = [%getenv "USER"; "default value"];

let var3: option(string) = [%getenv "HOME"];

let var4: option(string) = [%getenv "SOME_VAR_THAT_DOESNT_EXIST"];
```

## Developing

Clone repo and install deps:

```shell
esy install
yarn install
```

Build ppx:

```shell
esy build
```

To explore generated output, run `yarn build` and look the compiled file `test/Test.bs.js`.
