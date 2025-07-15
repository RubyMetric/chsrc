# Global Config

- prefix = `TEST_`
- output = `:terminal`
- translate = `:escape`

## Java

### Maven Config

- name = `maven_settings`

```xml
<mirror>
  <id>test</id>
  <name>Test Mirror</name>
  <url>https://example.com</url>
</mirror>
```

### Gradle Config

```groovy
repositories {
  maven { url 'https://example.com' }
}
```

## Python

- prefix = `PY_`

### pip config

```ini
[global]
index-url = https://example.com/simple
```

### conda config

- language = `yaml`

```
channels:
  - https://example.com/conda
```

## Docker

### Dockerfile

```dockerfile
FROM ubuntu:20.04
RUN echo "Hello World"
```

#### Multi-stage Build

```dockerfile
FROM node:16 AS builder
WORKDIR /app
COPY . .
RUN npm install
```
