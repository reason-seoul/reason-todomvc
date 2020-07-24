type t = {
  id: int,
  text: string,
  completed: bool,
};

let make: (~text: string) => t;