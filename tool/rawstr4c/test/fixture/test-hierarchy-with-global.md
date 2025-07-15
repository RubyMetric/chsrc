- global-prefix = `GLOBAL_`
- global-output = `:terminal`

```c
int global_code_block = 1;
```

some comments



# Java

- java-prefix = `JAVA_`

## Maven Config

- maven-name = `maven`

```xml
<settings>
  <mirrors>
  </mirrors>
</settings>
```

## Gradle Config

```groovy
repositories {
  maven { url 'https://example.com' }
}
```



# Python

- python-prefix = `PYTHON_`

## pip config

```bash
pip config set global.index-url https://example.com
```

## conda config

- language = `yaml`

```
channels:
  - https://example.com/conda
```



# Docker

## Dockerfile

```dockerfile
FROM ubuntu:20.04
RUN echo "Hello World"
```

### Multi-stage Build

```dockerfile
FROM node:16 AS builder
WORKDIR /app
COPY . .
RUN npm install
```
