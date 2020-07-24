module Todo = Model_Todo;
module AppState = Model_AppState;

[@react.component]
let make = (~todo: Todo.t) => {
  let dispatch = AppState.Dispatch.use();

  <div className="view">
    <input
      className="toggle"
      type_="checkbox"
      checked={todo.completed}
      onChange={event => {
        let value = event->ReactEvent.Form.target##checked;
        dispatch(TodoCompleted(todo.id, value));
      }}
    />
    <label> {React.string(todo.text)} </label>
    <button
      className="destory"
      onClick={_ => {dispatch(DestroyTodo(todo.id))}}
    />
  </div>;
};
