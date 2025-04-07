
(*coder l'interpreter en ocaml avant de le fair en eclat car il y une etape pour aplatir le bytecode vers eclat car il change un peu *)
(*ne pas hesiter a poser des questions pour le makefile avce le lexer et le parser*)
(*ce serait cool de fair toutes les etapes: lexer,parser puis vers CAM*)

(* from a 1986 paper: https://www.cs.tufts.edu/~nr/cs257/archive/dominique-clement/applicative.pdf *)

(* figure 1 page 14 (Abstract Syntax of Mini-ML) *)
type ident = string

type pat =
| Pairpat of pat * pat
| IdentPat of ident
| NullPat

type expr =
| Ident of ident
| Number of int
| False
| True
| Apply of expr * expr
| Mlpair of expr * expr
| Lambda of pat * expr
| Let of pat * expr * expr
| LetRec of pat * expr * expr
| If of expr * expr * expr


(* figure 7 page 21 (Abstract syntax of CAM code) *)

type program = coms
and coms = com list
and com =
| Quote of value
| Op of operator
| Car
| Cdr
| Cons
| Push
| Swap
| App
| Rplac
| Cur of coms
| Branch of coms * coms
and value =
| Int of int
| Bool of bool
| NullValue

and operator = Add | Sub | Mult


(* Figure 10 page 24 (Translation from Mini-ML to CAM) *)
let rec compile (e : expr) (env : (ident * value) list)  : coms =
  match e with
  | Number(n) -> [Quote(Int(n))]
  | True -> [Quote(Bool(true))]
  | False -> [Quote(Bool(false))]
  
  | Ident(x) ->  let v = List.assoc x env in [Quote(v)]
      
  
  | If(a, b, c) -> 
      let c1 = compile a env in
      let c2 = compile b env in
      let c3 = compile c env in
      Push :: c1 @ [Branch(c2, c3)]

  
  | Lambda(p, body) ->
      let new_env = match p with
        | IdentPat(x) -> (x, NullValue) :: env
        | _ -> env 
      in
      let c_body = compile body new_env in
      [Cur(c_body)]
  
  | Let(p, e1, e2) ->
      let c1 = compile e1 env in
      let new_env = match p with
        | IdentPat(x) -> (x, NullValue) :: env
        | _ -> env 
      in
      let c2 = compile e2 new_env in
      Push :: c1 @ [Cons] @ c2
  
  | LetRec(p, e1, e2) ->
      let new_env = match p with
        | IdentPat(x) -> (x, NullValue) :: env
        | _ -> env
      in
      let c1 = compile e1 new_env in
      let c2 = compile e2 new_env in
     Push :: [Quote(NullValue); Cons; Push] @ c1 @ [Swap; Rplac] @ c2

 |Mlpair(e1,e2)-> let c1= compile e1 env in 
                  let c2= compile e2 env in 
                  [Push]@c1@[Swap]@c2@[Cons]

  |Apply(e1,e2) -> (match e1 with
                          |Ident("+")-> compile e2 env @[Op(Add)]
                          |Ident("-")-> compile e2 env @[Op(Sub)]
                          |Ident("*")-> compile e2 env @[Op(Mult)]
                          |_-> let c1= compile e1 env in 
                               let c2= compile e2 env in 
                               [Push]@c1@[Swap]@c2@[Cons;App])
  
                     

 



let string_of_value = function
  | Int(n) -> string_of_int n
  | Bool(true) -> "true"
  | Bool(false) -> "false"
  | NullValue -> "null"




let string_of_operator = function
  | Add -> "+"
  | Sub -> "-"
  | Mult -> "*"



let rec string_of_com = function
  | Quote(v) -> "Quote(" ^ string_of_value v ^ ")"
  | Op(op) -> "Op(" ^ string_of_operator op ^ ")"
  | Car -> "Car"
  | Cdr -> "Cdr"
  | Cons -> "Cons"
  | Push -> "Push"
  | Swap -> "Swap"
  | App -> "App"
  | Rplac -> "Rplac"
  | Cur(c) -> "Cur(" ^ string_of_coms c ^ ")"
  | Branch(c1, c2) ->
      "Branch(" ^ string_of_coms c1 ^ ", " ^ string_of_coms c2 ^ ")"

and string_of_coms cs =
  "[" ^ String.concat "; " (List.map string_of_com cs) ^ "]"



let test_expr = Apply(Ident("+"), Mlpair(Number(2), Number(3)));;
let cam_code = compile test_expr [];;

print_endline (string_of_coms cam_code);;
