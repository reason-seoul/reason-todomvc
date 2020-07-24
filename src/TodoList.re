open Model_Todo;
open Model_Filter;

[@react.component]
let make = (~todos, ~filter) => {
  <ul className="todo-list">
    {todos
      ->Belt.Array.keep(todo =>
          switch (filter, todo.completed) {
          | (Completed, true) => true
          | (Active, false) => true
          | (All, _) => true
          | _ => false
          }
        )
      ->Belt.Array.map(todo => {
          <li key={todo.id->string_of_int}> <TodoItem todo /> </li>
        })
      ->React.array}
  </ul>
};