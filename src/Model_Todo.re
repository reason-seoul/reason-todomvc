type t = {
  id: int,
  text: string,
  completed: bool,
};

let id_gen = ref(0);

let make = (~text) => {
  let id = id_gen^;
  id_gen := id + 1;

  {id, text, completed: false};
};
