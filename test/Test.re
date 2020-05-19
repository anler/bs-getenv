let getDefaultValue = () => "Some other value";

let var1: string = [%getenv SOME_VAR_THAT_DOESNT_EXIST; getDefaultValue()];
let var2: string = [%getenv USER; "default value"];

let var3: option(string) = [%getenv HOME];
let var4: option(string) = [%getenv SOME_VAR_THAT_DOESNT_EXIST];

type env =
  | Dev
  | CI
  | Prod;

let var5: env =
  switch%getenv (ENV) {
  | "ci" => CI
  | "dev" => Dev
  | _ => Prod
  };
let var6: option(env) =
  switch%getenv (ENV) {
  | "prod" => Prod
  };
