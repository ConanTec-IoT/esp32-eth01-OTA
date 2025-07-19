<img width=100% src="https://capsule-render.vercel.app/api?type=waving&color=C62718&height=120&section=header"/>

# ESP32-ETH01 com OTA (Over-the-Air)

Este repositório apresenta um exemplo funcional utilizando o módulo **ESP32-ETH01**, demonstrando como realizar **atualizações de firmware via OTA (Over-the-Air)** por meio da interface Ethernet.

O projeto inclui código compatível tanto com a **Arduino IDE** quanto com o **PlatformIO**, destacando as diferenças e particularidades de cada ambiente de desenvolvimento.

#

## 🔌 Gravação Inicial via Serial

Antes de realizar atualizações OTA, é necessário gravar o firmware inicial via interface **serial**, utilizando um conversor USB-Serial (como **FTDI** ou **CH340**), pois o suporte OTA não vem de fábrica no ESP32.

### Conexões recomendadas:

| Pino ESP32 | Pino Conversor | Observações                                                                |
| ---------- | -------------- | -------------------------------------------------------------------------- |
| TX (IO1)   | RX             | Comunicação serial                                                         |
| RX (IO3)   | TX             | Comunicação serial                                                         |
| IO0        | GND            | Deve ser conectado ao GND ao ligar o módulo para entrar em modo bootloader |
| 5V         | 5V             | Recomenda-se uso de fonte externa (não diretamente pelo conversor)         |
| GND        | GND            | Todos os GNDs devem estar interligados                                     |

> [!NOTE]
> Após a gravação, desconecte o IO0 do GND e reinicie o módulo para que ele inicie no modo normal de operação.

#

## 🌐 Diferenças entre Arduino IDE e PlatformIO

* **Arduino IDE:**
  Utiliza bibliotecas **assíncronas**, como a `ESPAsyncWebServer`, proporcionando melhor desempenho na manipulação de conexões simultâneas.

* **PlatformIO:**
  Não foi possível utilizar bibliotecas assíncronas neste ambiente, devido a problemas de compatibilidade.
  Por isso, o projeto utiliza o servidor padrão síncrono com `WebServer.h`.

#

## ⚠️ Observação sobre a conexão Ethernet

Durante os testes, observou-se que o **ESP32-ETH01 não conseguia se conectar diretamente a alguns roteadores**, tanto via DHCP quanto com IP estático.
A conexão só foi bem-sucedida ao conectar o módulo a um **switch**, que então fazia a ponte com o roteador.

---

## 📎 Links úteis

* [Guia não oficial do WT32-ETH01 (por egnor)](https://github.com/egnor/wt32-eth01)
* [Datasheet oficial do módulo](https://files.seeedstudio.com/products/102991455/WT32-ETH01_datasheet_V1.1-%20en.pdf)
