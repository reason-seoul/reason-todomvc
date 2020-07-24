# Reason Seoul Workshop #1

[![Join Meetup](https://img.shields.io/badge/Meetup-Join%20us-%23f65858)](https://www.meetup.com/Reason-Seoul/)
[![Discord](https://img.shields.io/discord/717436122480902225.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/RzShCNp)
[![Twitter Follow](https://img.shields.io/twitter/follow/ReasonSeoul.svg?style=social)](https://twitter.com/ReasonSeoul)

Reason Seoul 커뮤니티에 놀러오세요!

## Reason React로 TodoMVC 만들기

워크샵 기록:

- [Reason React로 TodoMVC 만들기
](https://www.meetup.com/Reason-Seoul/events/271960396/)
- (TBD) Youtube

### 둘러보기

#### 초기화

이 저장소를 클론합니다.

```bash
git clone https://github.com/reason-seoul/reason-todomvc.git
```

저장소는 탄소를 절감하고 우리들의 SSD 수명을 조금 더 보존하기 위해 [pnpm](https://pnpm.js.org/) 패키지 매니져를 사용하고 있습니다 :wink:

```bash
# 아직 pnpm이 없다구요? 바로 설치하세요!
# npx pnpm add -g pnpm

# 이제부턴 npx도 pnpx로 바꿔씁니다.

pnpm install
```

코드를 직접 작성하고 싶은 분들은 `basic-setup` 태그를 체크아웃 하세요! 제가 여러분들을 위해 귀찮은 웹 팩 설정을 대신 해드렸답니다.

```bash
git checkout basic-setup
```

#### 이게 뭔가요?

##### BuckleScript

[BuckleScript](https://bucklescript.github.io/)는 OCaml/Reason 으로 작성된 코드를 JavaScript 로 바꿔주는 컴파일러입니다.

```bash
pnpm add bs-platform
```

다른 도구들과 다르게 표현식 레벨에서의 변환만 일어나고, Minify등의 처리는 직접 다루지 않습니다.

Highlights:

- 컴파일 속도가 눈부시게 빠르고,
- Dead-code elimination, Inline caching optimization, recursion optimization 등의 최적화가 적용되면서,
- 여전히 읽을 수(!) 있는 JavaScript 코드가 생성됩니다.

##### Belt

[Belt](https://reasonml.org/apis/javascript/latest/belt)는 BuckleScript에 포함된 표준라이브러리 입니다. Array/List/Option 등의 표준 자료형을 다루기 위한 라이브러리들을 제공하고 있습니다.

##### Reason React

[Reason React](https://reasonml.github.io/reason-react/)는 Reason 커뮤니티가 관리하는 React 바인딩입니다.

Reason은 JSX를 지원하며, React의 API를 거의 유사한 방식으로 지원하기 때문에 JavaScript와 차이점이 거의 방식으로 React 앱을 만들 수 있습니다.

```re
module Hello = {
  [@react.component]
  let make = (~name) => {
    <div>{React.string(name)}</div>;
  };
};

[@react.component]
let make = () => {
  let name = "World!";
  <Hello name>;
};
```

**Fun fact 1:** 그거 아시나요? [React 의 초기 프로토타입은 JavaScript 보다 Reason에 더 가까웠습니다!](https://dev.to/andrefbsantos/reasonml-react-as-first-intended-2020-25j0) (완성이 가까워지면서 JavaScript로 재작성되었습니다) Reason React 는 종종 React 보다 더 낫게 느껴집니다.

**Fun fact 2:** Reason React 의 API는 React보다 발전이 빠릅니다. Hooks가 도입되기 전에 [Subscription Helper](https://reasonml.github.io/reason-react/docs/en/subscriptions-helper#docsNav) API가 존재했고, Reducer가 컴포넌트 기본 프리미티브로 제공되기 전부터 [Reducer Component](https://reasonml.github.io/reason-react/docs/en/state-actions-reducer)를 제공했습니다. 지금은 [Concurrent Mode API를 *공식*으로 먼저 지원](https://reasonml.github.io/reason-react/blog/2020/05/05/080-release)합니다!

### 타입

Reason은 정말 강력한 타입 시스템을 통해 프로그램의 안정성을 보장합니다.

```re
type todo = {
  id: int,
  completed: bool,
  text: string,
};

type action =
  | NewTodo(string)
  | CompleteTodo(int);

type state = {
  todos: array(todo),
};
```

Record 와 Tuple 타입을 통해 기본적으로 **불변인 값**들을 다루고, ADT를 활용해서 액션을 모델링 할 수 있습니다.

그리고 모든 Reason 타입을 **패턴 매칭**이 가능합니다.

ADT 매칭을 이용한 리듀서:

```re
open Belt;

let reducer = (state, action) => {
  switch (action) {
  | NewTodo(text) => {
      todos: state.todos->Array.concat([| Todo.make(~text) |]),
    }
  | TodoCompleted(id, completed) => {
      todos:
        state.todos
        ->Array.map(todo => todo.id == id ? {...todo, completed} : todo),
    }
};
```

튜플 매칭을 이용한 투두 아이템 필터:

```re
// Reason React는 라우터도 기본으로 제공합니다.
let url = ReasonReactRouter.useUrl();

type filter =
  | All
  | Active
  | Completed;

let filter =
  switch (url.hash) {
  | "active" => Active
  | "completed" => Completed
  | _ => All
  };

// 와 너무 직관적이다
state.todos
->Array.keep(todo => switch (filter, todo) {
  | (Completed, true) => true
  | (Active, false) => true
  | (All, _) => true
  | _ => false
})
```

그냥 투두 레코드 필터:

```re
let isFirstThree = switch (todo) {
  | { id: 1 | 2 | 3, _ } => true
  | _ => false
};
```

그런데 타입 어노테이션들은 다 어디갔나요?

걱정 마세요! Reason은 여러분이 무엇을 원하는지 이미 다 눈치챘답니다!

Reason의 강력한 타입 추론으로 여러분의 손가락 관절을 구원하세요.

### 모듈 시스템

Reason의 모듈 시스템은 정말 강력해서, 여러분을 파일 당 수십 라인의 `import` 문을 타이핑 하는 고통해서 해방시켜줍니다.

여러분이 만든 파일은 이미 하나의 식별가능한 모듈입니다! (그렇기 때문에 파일명은 프로젝트 안에서 고유해야 합니다.) `module`을 통해 직접 모듈을 정의할 수도 있습니다.

```re
// Model.re

module Todo = {
  type t = {
    // ...
  };
  let make = () => { /* ... */ }
}
```

이제 다른 프로젝트의 어떤 곳에서든 `Model.Todo.t` 타입에 바로 접근할 수 있고, 모듈을 `open`해서 내부의 타입에 접근할 수도 있습니다.

```re
// Whereever
type t = Model.Todo.t;

module Nested = {
  open Model;
  type t = Todo.t;
};
```
