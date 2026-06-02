# Patrones Reutilizables

## Adaptar Patron Fluido a Plugins .irx (CAD)

- Usar un `DbFlow<T>` centrado en transacciones y estado de error, no solo en tipos simples.
- Encadenar sobre `ObjectId` o colecciones de IDs.
- Abrir entidades en scopes cortos (RAII) y no retener punteros fuera de la transaccion.
- Modelar cada paso con `Result<T, E>` para que `map` y `flatMap` solo avancen en exito.
- Si falla un paso de escritura, cortar la cadena y aplicar rollback.

## Pipeline Recomendado

1. Seleccionar IDs objetivo.
2. Filtrar por capa, tipo o metadata (XData).
3. Abrir en lectura o escritura segun corresponda.
4. Transformar o actualizar entidades.
5. Persistir cambios.
6. Commit final.

## Operadores Sugeridos

- `whereLayer`
- `whereType`
- `forWrite`
- `mapEach`
- `tap(log)`
- `commit`
- `recover` / `orElse`

## Reglas Criticas

- Escribir siempre dentro de transaccion.
- No mezclar entidades de distintas bases de datos en la misma cadena.
- En operaciones masivas, procesar por lotes para controlar memoria y tiempos.
