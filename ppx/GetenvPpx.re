open Migrate_parsetree;
open Ast_406;
open Ast_mapper;
open Ast_helper;
open Parsetree;

let getEnv = name => {
  let value = Sys.getenv(name);
  Exp.constant(Pconst_string(value, None));
};

let resultMapper = (config, cookies) => {
  ...default_mapper,
  expr: (mapper, expr) =>
    switch (expr) {
    | {
        pexp_desc:
          Pexp_extension((
            {txt: "getenv"},
            PStr([
              {
                pstr_desc:
                  Pstr_eval(
                    {pexp_desc: Pexp_constant(Pconst_string(name, None))},
                    _,
                  ),
              },
              {pstr_desc: Pstr_eval(default, _)},
            ]),
          )),
      } =>
      try(getEnv(name)) {
      | Not_found => default
      }
    | {
        pexp_desc:
          Pexp_extension((
            {txt: "getenv"},
            PStr([
              {
                pstr_desc:
                  Pstr_eval(
                    {pexp_desc: Pexp_constant(Pconst_string(name, None))},
                    _,
                  ),
              },
            ]),
          )),
      } =>
      try(
        Exp.construct(
          Location.mknoloc(Longident.Lident("Some")),
          Some(getEnv(name)),
        )
      ) {
      | Not_found =>
        Exp.construct(Location.mknoloc(Longident.Lident("None")), None)
      }
    | _ => default_mapper.expr(mapper, expr)
    },
};

let () =
  Driver.register(~name="bs-getenv-ppx", Versions.ocaml_406, resultMapper);
