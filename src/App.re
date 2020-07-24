open Model_Filter;
module AppState = Model_AppState;

module Counter = {
  [@react.component]
  let make = (~todos) => {
    <span className="todo-count">
      <strong> {todos->Belt.Array.length->React.int} </strong>
      {React.string(" items left")}
    </span>;
  };
};

module Filters = {
  [@react.component]
  let make = () => {
    <ul className="filters">
      <li> <a href="#"> {React.string("All")} </a> </li>
      <li> <a href="#active"> {React.string("Active")} </a> </li>
      <li> <a href="#completed"> {React.string("Completed")} </a> </li>
    </ul>;
  };
};

[@react.component]
let make = () => {
  let (text, setText) = React.useState(() => "");
  let (state, dispatch) = React.useReducer(AppState.reducer, {todos: [||]});

  let url = ReasonReactRouter.useUrl();
  let filter =
    switch (url.hash) {
    | "active" => Active
    | "completed" => Completed
    | _ => All
    };

  <AppState.Dispatch value=dispatch>
    <section className="todoapp">
      <header className="header">
        <h1> {React.string("TodoMVC")} </h1>
        <form
          onSubmit={event => {
            event->ReactEvent.Form.preventDefault;
            if (text->String.length > 0) {
              dispatch(NewTodo(text));
              setText(_ => "");
            };
          }}>
          <input
            className="new-todo"
            value=text
            onChange={event => {
              let newText = event->ReactEvent.Form.target##value;
              setText(_ => newText);
            }}
          />
        </form>
      </header>
      <main className="main">
        <TodoList todos=state.todos filter />
      </main>
      <footer> <Counter todos={state.todos} /> <Filters /> </footer>
    </section>
  </AppState.Dispatch>;
};
