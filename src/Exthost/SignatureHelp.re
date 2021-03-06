open Oni_Core;

module ProviderMetadata = {
  [@deriving show]
  type t = {
    triggerCharacters: list(string),
    retriggerCharacters: list(string),
  };

  let decode =
    Json.Decode.(
      obj(({field, _}) =>
        {
          triggerCharacters:
            field.withDefault("triggerCharacters", [], list(string)),
          retriggerCharacters:
            field.withDefault("retriggerCharacters", [], list(string)),
        }
      )
    );
};

module TriggerKind = {
  [@deriving show]
  type t =
    | Invoke
    | TriggerCharacter
    | ContentChange;

  let toInt =
    fun
    | Invoke => 1
    | TriggerCharacter => 2
    | ContentChange => 3;

  let ofInt =
    fun
    | 1 => Some(Invoke)
    | 2 => Some(TriggerCharacter)
    | 3 => Some(ContentChange)
    | _ => None;

  let encode = triggerKind => triggerKind |> toInt |> Json.Encode.int;
};

module RequestContext = {
  [@deriving show]
  type t = {
    triggerKind: TriggerKind.t,
    triggerCharacter: option(string),
    isRetrigger: bool,
    // TODO: Active signature help?
    //activate
  };

  let encode = ctx =>
    Json.Encode.(
      obj([
        ("triggerKind", ctx.triggerKind |> TriggerKind.encode),
        ("triggerCharacter", ctx.triggerCharacter |> nullable(string)),
        ("isRetrigger", ctx.isRetrigger |> bool),
      ])
    );
};
module ParameterInformation = {
  [@deriving show]
  type t = {
    label: string,
    // TODO:
    //documentation: option(MarkdownString.t),
  };

  let decode =
    Json.Decode.(
      obj(({field, _}) =>
        {
          label: field.required("label", string),
          //documentation: field.optional("documentation", string),
        }
      )
    );
};

module Signature = {
  [@deriving show]
  type t = {
    label: string,
    // TODO:
    //documentation: options(MarkdownString.t),
    parameters: list(ParameterInformation.t),
  };

  let decode =
    Json.Decode.(
      obj(({field, _}) =>
        {
          label: field.required("label", string),
          parameters:
            field.withDefault(
              "parameters",
              [],
              list(ParameterInformation.decode),
            ),
        }
      )
    );
};

module Response = {
  [@deriving show]
  type t = {
    id: int,
    signatures: list(Signature.t),
    activeSignature: int,
    activeParameter: int,
  };

  let decode =
    Json.Decode.(
      obj(({field, _}) =>
        {
          id: field.required("id", int),
          signatures: field.required("signatures", list(Signature.decode)),
          activeSignature: field.required("activeSignature", int),
          activeParameter: field.required("activeParameter", int),
        }
      )
    );
};
