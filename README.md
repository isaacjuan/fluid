# Ejemplo Fluido en C++

Proyecto didactico para practicar composicion y encadenamiento de operaciones en C++17.

Incluye tres enfoques complementarios:

- `Composable`: estilo clasico orientado a objeto mutable con metodos que retornan `*this`.
- `Fluent<T>`: pipeline fluido y tipado para transformar valores paso a paso.
- `StaticFluent<T>`: funciones estaticas para un estilo funcional sin mantener estado interno.

## Objetivo

Mostrar como modelar pipelines con transformaciones, efectos laterales y control de ausencia de valor usando `std::optional`.

## Operaciones disponibles en Fluent<T>

- `from(value)`: fabrica estatica para iniciar una cadena sin variable intermedia.
- `map(f)`: transforma `T -> U` y devuelve `Fluent<U>`.
- `flatMap(f)`: encadena funciones que ya devuelven `Fluent<U>` sin anidar `Fluent<Fluent<U>>`.
- `apply(f)`: permite mutar el valor interno (recibe `T&`) y continuar la cadena.
- `tap(f)`: observa el valor (por ejemplo, log) sin transformarlo.
- `then(f)`: alias semantico de `map` para lectura tipo pipeline.
- `flatten()`: aplana `Fluent<Fluent<U>> -> Fluent<U>`.
- `filter(pred)`: convierte `Fluent<T>` en `Fluent<std::optional<T>>`.
- `mapIfPresent(f)`: aplica transformacion solo cuando el `optional` contiene valor.
- `orElse(defaultValue)`: recupera un `T` cuando el `optional` esta vacio.
- `get()`: extrae el valor final.

## Flujo didactico en main

El `main` esta organizado por bloques numerados para recorrer conceptos en orden:

1. Chaining clasico con `Composable`.
2. `apply + map` en `Fluent<int>`.
3. Inicializacion con `Fluent<int>::from(...)`.
4. Cambio de contexto con `flatMap` (`int -> string`).
5. Observabilidad con `tap` y continuacion con `then`.
6. Aplanado con `flatten`.
7. Filtrado con `filter` y transformacion condicional con `mapIfPresent`.
8. Recuperacion de fallback con `orElse`.
9. Variante estatica con `StaticFluent<T>`.

## Compilar y ejecutar

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o fluid_demo
./fluid_demo
```

## Salida esperada (aprox.)

```text
Value: 15, Text: Hello World!
169
169
Result: 10
tap: 10
15
43
24
-1
25
100
169
```

## Ideas para extender

- `orElseGet(...)`: fallback perezoso calculado solo cuando no hay valor.
- `recover(...)`: recuperacion a partir de estrategia o funcion.
- `zip(...)`: combinar dos pipelines en uno.
- Integrar manejo de errores explicito con un tipo `Result<T, E>`.
