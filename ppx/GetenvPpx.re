open Migrate_parsetree;
open Ast_406;
open Ast_mapper;
open Ast_helper;
open Parsetree;

let getEnv = name => {
  let value = Sys.getenv(name);
  Exp.constant(Pconst_string(value, None));
};

let rec checkCases =
  fun
  | [{pc_lhs: {ppat_loc: loc, ppat_desc: Ppat_var(_)}}, ...rest] =>
    raise(
      Location.Error(
        Location.error(~loc, "%getenv only supports string constants"),
      ),
    )
  | [_, ...rest] => checkCases(rest)
  | [] => ();

let rec matchesValue = value =>
  fun
  | {ppat_desc: Ppat_constant(Pconst_string(constValue, None))} =>
    constValue == value
  | {ppat_desc: Ppat_or(left, right)} =>
    matchesValue(value, left) || matchesValue(value, right)
  | _ => false;

let matches = (value, {pc_lhs: expr}) => matchesValue(value, expr);

let getMatchExp = case_ => case_.pc_rhs;

let match = (value, cases) =>
  cases |> List.find(matches(value)) |> getMatchExp;

let getEnvMatch = (name, cases) => {
  let value = Sys.getenv(name);
  match(value, cases);
};

let isAnyCase =
  fun
  | {pc_lhs: {ppat_desc: Ppat_any}} => true
  | _ => false;

let getAnyCase = cases =>
  try(Some(cases |> List.find(isAnyCase))) {
  | Not_found => None
  };

let resultMapper = (config, cookies) => {
  ...default_mapper,
  expr: (mapper, expr) =>
    switch (expr) {
    | {
        pexp_desc:
          Pexp_extension((
            {txt: "getenv.exn"},
            PStr([
              {
                pstr_desc:
                  Pstr_eval(
                    {
                      pexp_loc: loc,
                      pexp_desc: Pexp_construct({txt: Lident(name)}, _),
                    },
                    _,
                  ),
              },
            ]),
          )),
      } =>
      try(getEnv(name)) {
      | Not_found =>
        raise(
          Location.Error(
            Location.error(
              ~loc,
              "%getenv environment variable not found: " ++ name,
            ),
          ),
        )
      }
    | {
        pexp_desc:
          Pexp_extension((
            {txt: "getenv"},
            PStr([
              {
                pstr_desc:
                  Pstr_eval(
                    {pexp_desc: Pexp_construct({txt: Lident(name)}, _)},
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
                    {pexp_desc: Pexp_construct({txt: Lident(name)}, _)},
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
    | {
        pexp_loc: loc,
        pexp_desc:
          Pexp_extension((
            {txt: "getenv"},
            PStr([
              {
                pstr_desc:
                  Pstr_eval(
                    {
                      pexp_desc:
                        Pexp_match(
                          {
                            pexp_desc:
                              Pexp_construct({txt: Lident(name)}, _),
                          },
                          cases,
                        ),
                    },
                    _,
                  ),
              },
            ]),
          )),
      } =>
      cases |> checkCases;
      switch (cases |> getAnyCase) {
      | None =>
        try(
          Exp.construct(
            Location.mknoloc(Longident.Lident("Some")),
            Some(getEnvMatch(name, cases)),
          )
        ) {
        | Not_found =>
          Exp.construct(Location.mknoloc(Longident.Lident("None")), None)
        }
      | Some({pc_rhs: default}) =>
        try(getEnvMatch(name, cases)) {
        | Not_found => default
        }
      };
    | {pexp_loc: loc, pexp_desc: Pexp_extension(({txt: "getenv"}, _))} =>
      raise(
        Location.Error(
          Location.error(~loc, "Unsupported %getenv expression"),
        ),
      )
    | _ => default_mapper.expr(mapper, expr)
    },
};

let () =
  Driver.register(~name="bs-getenv-ppx", Versions.ocaml_406, resultMapper);
