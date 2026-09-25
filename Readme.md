# 💾 Central Dados

> **Central de dados embarcada baseada em ESP32 + MicroSD + Wi-Fi, com gerenciamento de arquivos através de uma interface Web.**

![ESP32](https://img.shields.io/badge/ESP32-Embedded-red?style=for-the-badge\&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-Framework-00979D?style=for-the-badge\&logo=arduino)
![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-orange?style=for-the-badge\&logo=platformio)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge\&logo=cplusplus)
![MicroSD](https://img.shields.io/badge/MicroSD-Storage-green?style=for-the-badge)
![Wi-Fi](https://img.shields.io/badge/Wi--Fi-802.11n-purple?style=for-the-badge)

---

## 📋 Índice

* [Sobre o projeto](#-sobre-o-projeto)
* [Objetivos](#-objetivos)
* [Principais funcionalidades](#-principais-funcionalidades)
* [Arquitetura](#-arquitetura)
* [Fluxo da aplicação](#-fluxo-da-aplicação)
* [Tecnologias](#-tecnologias)
* [Hardware](#-hardware)
* [Pinagem do MicroSD](#-pinagem-do-microsd)
* [Estrutura do projeto](#-estrutura-do-projeto)
* [Interface Web](#-interface-web)
* [API REST](#-api-rest)
* [Configuração](#-configuração)
* [Compilação e upload](#-compilação-e-upload)
* [Execução](#-execução)
* [Modo Access Point](#-modo-access-point)
* [Segurança](#-segurança)
* [Evolução do projeto](#-evolução-do-projeto)
* [Aplicações](#-aplicações)
* [Roadmap](#-roadmap)
* [Autor](#-autor)
* [Licença](#-licença)

---

# 🚀 Sobre o projeto

O **Central Dados** é um projeto de Internet das Coisas (IoT) desenvolvido sobre a plataforma **ESP32**, com o objetivo de transformar o microcontrolador em uma pequena central embarcada para armazenamento e gerenciamento de dados.

O dispositivo utiliza um **cartão MicroSD** como unidade de armazenamento e disponibiliza uma **interface Web responsiva**, acessível através da rede Wi-Fi.

A partir do navegador, é possível:

* 📁 visualizar arquivos;
* 📖 abrir arquivos;
* ✏️ editar arquivos de texto;
* 💾 salvar alterações;
* 📤 enviar arquivos para o MicroSD;
* ⬇️ baixar arquivos;
* 🗑️ excluir arquivos;
* 💽 visualizar informações de armazenamento;
* 🔄 atualizar a listagem sem reiniciar o dispositivo.

O projeto foi estruturado de forma modular para facilitar manutenção, evolução e reutilização do código.

---

# 🎯 Objetivos

O projeto foi desenvolvido com os seguintes objetivos:

* Aplicar conceitos de **sistemas embarcados**;
* Trabalhar com **ESP32 e armazenamento externo**;
* Desenvolver uma interface Web embarcada;
* Criar uma API HTTP para comunicação com o dispositivo;
* Trabalhar com manipulação de arquivos em MicroSD;
* Aplicar organização modular em C++;
* Criar uma solução que possa ser utilizada como base para outros projetos IoT.

---

# ✨ Principais funcionalidades

| Funcionalidade    | Descrição                                   |
| ----------------- | ------------------------------------------- |
| 📁 Listagem       | Lista os arquivos armazenados no MicroSD    |
| 📖 Leitura        | Abre arquivos diretamente pelo navegador    |
| ✏️ Edição         | Permite editar arquivos de texto            |
| 💾 Salvamento     | Salva alterações diretamente no MicroSD     |
| 📤 Upload         | Envia arquivos do computador para o ESP32   |
| ⬇️ Download       | Baixa arquivos armazenados no dispositivo   |
| 🗑️ Exclusão      | Remove arquivos do MicroSD                  |
| 💽 Informações    | Exibe espaço total, utilizado e livre       |
| 📱 Responsividade | Interface adaptada para dispositivos móveis |
| 📡 Wi-Fi          | Conexão com rede Wi-Fi existente            |
| 📶 Access Point   | Cria uma rede própria caso o Wi-Fi falhe    |

---

# 🏗️ Arquitetura

A arquitetura foi projetada para separar as responsabilidades do sistema.

```text
                    ┌──────────────────────┐
                    │      Navegador       │
                    │  PC / Smartphone     │
                    └──────────┬───────────┘
                               │
                               │ HTTP
                               ▼
                    ┌──────────────────────┐
                    │      ESP32            │
                    │                      │
                    │   Web Server         │
                    │   REST API            │
                    └──────────┬───────────┘
                               │
                               │ SPI
                               ▼
                    ┌──────────────────────┐
                    │      MicroSD         │
                    │                      │
                    │   Arquivos / Dados   │
                    └──────────────────────┘
```

---

# 🔄 Fluxo da aplicação

```text
┌─────────────────────┐
│       ESP32         │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Inicializa Wi-Fi    │
└──────────┬──────────┘
           │
           ├───────────────┐
           │               │
           ▼               ▼
     Wi-Fi conectado    Falha Wi-Fi
           │               │
           │               ▼
           │        Access Point
           │               │
           └───────┬───────┘
                   ▼
          ┌─────────────────┐
          │ Inicializa SD   │
          └────────┬────────┘
                   │
                   ▼
          ┌─────────────────┐
          │ Web Server      │
          └────────┬────────┘
                   │
                   ▼
          ┌─────────────────┐
          │ Interface Web   │
          └────────┬────────┘
                   │
          ┌────────┴─────────┐
          ▼                  ▼
     REST API             MicroSD
```

---

# 🛠️ Tecnologias

## Firmware

* **C++**
* **Arduino Framework**
* **ESP32**
* **PlatformIO**

## Comunicação

* HTTP
* REST API
* Wi-Fi
* Access Point

## Armazenamento

* MicroSD
* SPI
* Sistema de arquivos compatível com ESP32

## Interface

* HTML5
* CSS3
* JavaScript
* WebServer embarcado

---

# 🔌 Hardware

O projeto foi desenvolvido utilizando:

| Componente          | Utilização                |
| ------------------- | ------------------------- |
| ESP32 WEMOS LOLIN32 | Microcontrolador          |
| MicroSD Module      | Armazenamento             |
| Cartão MicroSD      | Persistência dos dados    |
| Wi-Fi               | Comunicação de rede       |
| USB                 | Alimentação e programação |

---

# 📌 Pinagem do MicroSD

A comunicação com o MicroSD utiliza **SPI**.

| MicroSD |   ESP32 |
| ------- | ------: |
| CS      |  GPIO 5 |
| SCK     | GPIO 18 |
| MISO    | GPIO 19 |
| MOSI    | GPIO 23 |
| VCC     |    3.3V |
| GND     |     GND |

### Diagrama

```text
       MICROSD
    ┌─────────────┐
    │             │
CS  │─────────────│ GPIO 5
SCK │─────────────│ GPIO 18
MISO│─────────────│ GPIO 19
MOSI│─────────────│ GPIO 23
VCC │─────────────│ 3.3V
GND │─────────────│ GND
    │             │
    └─────────────┘
           │
           │ SPI
           ▼
      ┌───────────┐
      │   ESP32   │
      └───────────┘
```

---

# 📂 Estrutura do projeto

O código foi dividido em módulos para facilitar manutenção e evolução.

```text
Central-Dados/
│
├── include/
│   │
│   ├── config.h
│   ├── wifi_manager.h
│   ├── sd_manager.h
│   └── web_server.h
│
├── src/
│   │
│   ├── main.cpp
│   ├── wifi_manager.cpp
│   ├── sd_manager.cpp
│   └── web_server.cpp
│
├── platformio.ini
│
└── README.md
```

### Responsabilidade dos módulos

#### `main.cpp`

Responsável pelo ciclo principal da aplicação:

```text
setup()
   │
   ├── Wi-Fi
   ├── MicroSD
   └── Web Server
   │
   ▼
loop()
```

#### `wifi_manager`

Responsável por:

* conexão Wi-Fi;
* configuração do modo Station;
* criação do Access Point;
* identificação do IP.

#### `sd_manager`

Responsável por:

* inicialização do MicroSD;
* comunicação SPI;
* informações de armazenamento;
* estado do cartão.

#### `web_server`

Responsável por:

* servidor HTTP;
* páginas Web;
* API;
* arquivos;
* upload;
* download;
* edição;
* exclusão.

---

# 🌐 Interface Web

A interface foi desenvolvida para funcionar diretamente no navegador, sem necessidade de instalar um aplicativo adicional.

A página possui:

### Dashboard

Exibe:

```text
┌─────────────┬─────────────┬─────────────┐
│    TOTAL    │   UTILIZADO │    LIVRE    │
├─────────────┼─────────────┼─────────────┤
│   XX MB     │    XX MB    │    XX MB    │
└─────────────┴─────────────┴─────────────┘
```

### Gerenciamento de arquivos

```text
┌───────────────────────────────────────────────────────┐
│ 📁 Arquivos                              🔄 Atualizar │
├──────────┬────────┬─────────────┬─────────────────────┤
│ Arquivo  │ Tamanho│ Tipo        │ Ações               │
├──────────┼────────┼─────────────┼─────────────────────┤
│ dados.csv│ 12 KB  │ text/csv    │ Abrir Baixar Excluir│
└──────────┴────────┴─────────────┴─────────────────────┘
```

### Editor

Arquivos de texto podem ser abertos e modificados diretamente através do navegador.

---

# 🔌 API REST

A interface Web utiliza endpoints HTTP para comunicação com o ESP32.

| Método | Endpoint        | Função            |
| ------ | --------------- | ----------------- |
| `GET`  | `/`             | Página principal  |
| `GET`  | `/api/files`    | Lista arquivos    |
| `GET`  | `/api/read`     | Lê arquivo        |
| `POST` | `/api/save`     | Salva arquivo     |
| `GET`  | `/api/download` | Baixa arquivo     |
| `POST` | `/api/delete`   | Exclui arquivo    |
| `GET`  | `/api/info`     | Informações do SD |
| `POST` | `/api/upload`   | Envia arquivo     |

### Exemplo

Listagem:

```http
GET /api/files
```

Leitura:

```http
GET /api/read?path=/dados.txt
```

Exclusão:

```http
POST /api/delete
Content-Type: application/x-www-form-urlencoded

path=/dados.txt
```

---

# ⚙️ Configuração

As principais configurações ficam em:

```text
include/config.h
```

### Wi-Fi

```cpp
#define WIFI_SSID       "SEU_WIFI"
#define WIFI_PASSWORD   "SUA_SENHA"
```

### Access Point

```cpp
#define AP_SSID         "Central-Dados"
#define AP_PASSWORD     "12345678"
```

### MicroSD

```cpp
#define SD_CS_PIN       5
#define SD_SCK_PIN      18
#define SD_MISO_PIN     19
#define SD_MOSI_PIN     23
```

---

# 🧰 Compilação e upload

## Pré-requisitos

Instale:

* Visual Studio Code
* PlatformIO
* Driver USB do ESP32, quando necessário

Clone o projeto:

```bash
git clone https://github.com/SEU-USUARIO/Central-Dados.git
```

Entre no diretório:

```bash
cd Central-Dados
```

Compile:

```bash
pio run
```

Grave no ESP32:

```bash
pio run -t upload
```

Abra o monitor serial:

```bash
pio device monitor -b 115200
```

---

# ▶️ Execução

Após iniciar o ESP32, o firmware tenta conectar à rede configurada.

Exemplo:

```text
========================================
             CENTRAL DADOS
       WEMOS LOLIN32 / ESP32
========================================

========================================
             INICIANDO WI-FI
========================================

Conectando....
Wi-Fi conectado!

SSID: Rosimar2.4g
IP: 192.168.1.100

========================================
             INICIANDO MICROSD
========================================

MicroSD inicializado com sucesso!

========================================
          INICIANDO SERVIDOR WEB
========================================

Servidor Web iniciado!

Acesse:
http://192.168.1.100/
```

Abra o endereço exibido no navegador.

---

# 📶 Modo Access Point

Caso o ESP32 não consiga conectar à rede Wi-Fi configurada, o projeto possui um modo alternativo.

O ESP32 cria automaticamente:

```text
SSID: Central-Dados
Senha: 12345678
```

O endereço padrão do Access Point é:

```text
http://192.168.4.1/
```

Isso permite acessar a central mesmo quando não existe uma rede Wi-Fi disponível.

---

# 🔐 Segurança

O projeto possui validação dos caminhos recebidos pela API.

São bloqueados caminhos contendo:

```text
..
```

e:

```text
\
```

Também não é permitido excluir a raiz do MicroSD:

```text
/
```

Exemplo de validação:

```cpp
bool isSafePath(String path)
{
    path = normalizePath(path);

    if (!path.startsWith("/"))
        return false;

    if (path.indexOf("..") >= 0)
        return false;

    if (path.indexOf("\\") >= 0)
        return false;

    return true;
}
```

> **Observação:** este projeto foi desenvolvido para uso em ambiente controlado. Para disponibilização em redes não confiáveis, recomenda-se adicionar autenticação, autorização, HTTPS ou outras camadas de proteção.

---

# 🚀 Evolução do projeto

O **Central Dados** foi estruturado para servir como base para aplicações IoT mais complexas.

A arquitetura permite evoluir o projeto para trabalhar com diferentes tipos de dados.

Exemplos:

```text
ESP32
  │
  ├── Sensores
  │
  ├── MicroSD
  │
  ├── Wi-Fi
  │
  └── API HTTP
         │
         ▼
   Central Dados
         │
         ├── CSV
         ├── JSON
         ├── TXT
         └── Logs
```

---

# 💡 Aplicações

A arquitetura pode ser adaptada para diferentes cenários:

### 📊 Data Logging

Registro de:

* temperatura;
* umidade;
* pressão;
* luminosidade;
* tensão;
* corrente;
* dados de sensores.

### 🏭 IoT Industrial

Armazenamento local de:

* eventos;
* alarmes;
* telemetria;
* logs;
* medições.

### 🌐 Edge Computing

Processamento e armazenamento local antes do envio dos dados para uma plataforma externa.

### 🚗 Sistemas embarcados

Registro de informações durante a operação de equipamentos.

---

# 🗺️ Roadmap

O projeto pode evoluir para uma plataforma embarcada de dados mais completa.

* [x] Comunicação Wi-Fi
* [x] Access Point
* [x] Inicialização do MicroSD
* [x] Interface Web
* [x] Listagem de arquivos
* [x] Leitura de arquivos
* [x] Edição de arquivos
* [x] Salvamento
* [x] Download
* [x] Upload
* [x] Exclusão
* [x] Informações de armazenamento
* [x] Código modularizado
* [ ] Autenticação de usuários
* [ ] Gerenciamento de diretórios
* [ ] Criação de arquivos pela interface
* [ ] Criação de pastas
* [ ] Renomear arquivos
* [ ] Exclusão de diretórios
* [ ] Dashboard de sensores
* [ ] Registro automático de telemetria
* [ ] MQTT
* [ ] Integração com banco de dados
* [ ] API documentada
* [ ] Monitoramento do dispositivo
* [ ] OTA
* [ ] Sistema de logs
* [ ] Controle de permissões

---

# 📸 Demonstração

> Adicione aqui screenshots da interface Web e fotos do hardware.

Exemplo:

```text
docs/
├── central-dados-dashboard.png
├── central-dados-files.png
├── central-dados-editor.png
└── central-dados-hardware.jpg
```

Depois você pode adicionar:

```markdown
![Dashboard](docs/central-dados-dashboard.png)
```

---

# 🧠 Conceitos demonstrados

Este projeto demonstra conhecimentos em:

```text
┌─────────────────────────────────────────┐
│          CENTRAL DADOS                  │
├─────────────────────────────────────────┤
│                                         │
│  Embedded Systems                       │
│  C++                                    │
│  ESP32                                  │
│  PlatformIO                             │
│  SPI                                    │
│  MicroSD                                │
│  Wi-Fi                                  │
│  HTTP                                   │
│  REST API                               │
│  HTML / CSS / JavaScript                │
│  File System                            │
│  Modular Architecture                   │
│  IoT                                    │
│  Edge Computing                         │
│                                         │
└─────────────────────────────────────────┘
```

---

# 👨‍💻 Autor

**Thiago Vilarinho Lemes**

Engenheiro e Analista de Dados, com atuação em:

* Engenharia de Dados;
* Sistemas Embarcados;
* IoT;
* Python;
* C/C++;
* Cloud Computing;
* Data Engineering;
* Inteligência Artificial.

Este projeto faz parte do meu portfólio de desenvolvimento em **IoT, sistemas embarcados e engenharia de dados**.

---

# 📜 Licença

Este projeto está disponível para fins de estudo, desenvolvimento e experimentação.

Consulte o arquivo `LICENSE` para obter os termos completos de utilização.

---

<div align="center">

### 💾 Central Dados

**ESP32 • MicroSD • Wi-Fi • Web • IoT**

<br>

⭐ Se este projeto foi útil ou interessante, considere deixar uma estrela no repositório.

</div>
