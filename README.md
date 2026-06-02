# fluid

Proyecto de ejemplo en C++ para mostrar tres estilos de composicion:

- Metodo composable clasico (`Composable`)
- API fluida por instancia (`Fluent<T>`)
- Variante estatica utilitaria (`StaticFluent<T>`)

Tambien incluye una variante hibrida con entrada estatica y encadenado fluido:

- `Fluent<T>::from(...)`
- `transform(...)` para mapear valores
- `apply(...)` para mutar el estado interno
- `andThen(...)` para encadenar funciones que retornan otro `Fluent`

## Compilar y ejecutar

```bash
g++ -std=c++17 main.cpp -o fluid_demo
./fluid_demo
```

## Salida esperada (aprox.)

```text
Value: 15, Text: Hello World!
169
169
Result: 10
169
```
