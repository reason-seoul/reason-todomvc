module Todo = Model_Todo;

type t = {todos: array(Todo.t)};

type action =
  | NewTodo(string)
  | TodoCompleted(int, bool)
  | DestroyTodo(int);

let reducer = (state, action) => {
  switch (action) {
  | NewTodo(text) => {
      todos: state.todos->Belt.Array.concat([|Todo.make(~text)|]),
    }
  | TodoCompleted(id, completed) => {
      todos:
        state.todos
        ->Belt.Array.map(todo => todo.id == id ? {...todo, completed} : todo),
    }
  | DestroyTodo(id) => {
      todos: state.todos->Belt.Array.keep(todo => todo.id != id),
    }
  };
};

module Dispatch = {
  let context = React.createContext((_: action) => ());
  let make = React.Context.provider(context);
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };

  let use = () => React.useContext(context);
};
