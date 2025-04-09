# Sistema di Controllo LED con Sensore di Movimento PIR

## Descrizione

Questo progetto implementa un sistema di controllo di un LED tramite un sensore di movimento PIR utilizzando **Zephyr RTOS**. Il sistema funziona nel seguente modo:

- Quando il sensore PIR rileva un movimento, viene attivato un **callback** che aggiorna un timer e accende il LED.
- Il LED rimane acceso per **15 secondi** e si spegne automaticamente al termine di questo periodo. Se vengono rilevati nuovi movimenti durante i 15 secondi, il timer viene resettato e il LED rimane acceso per altri 15 secondi a partire dalla nuova rilevazione.
- La gestione del sensore PIR e del LED avviene tramite **GPIO** (General Purpose Input/Output), utilizzando un **interrupt** per rilevare il movimento.

Il progetto si basa su Zephyr RTOS per gestire la temporizzazione e la gestione degli interrupt.

## Tecnologie Utilizzate

- **Zephyr RTOS**: Sistema operativo in tempo reale per dispositivi embedded.
- **GPIO (General Purpose Input/Output)**: Per la gestione dei dispositivi hardware come il sensore PIR e il LED.
- **C**: Linguaggio di programmazione per lo sviluppo del sistema.

## Hardware Utilizzato

- **Espressif ESP32**: Scheda elettronica integrata supportata da **Zephyr RTOS**
- **Sensore PIR**: Sensore ad infrarossi passivo digitale
- **Breadboard + Jumpers**: Basetta sperimentale e ponticelli (jumpers) per il collegamento dei componenti HW
- **LED 5mm + Resistore**: Diodo led con resistore in serie per limitare la corrente

## Getting Started

Per iniziare con il progetto, segui i passaggi descritti di seguito per configurare l'ambiente di sviluppo e compilare il progetto.

### 1. Preparare l'ambiente di sviluppo

Prima di tutto, assicurati di avere Zephyr RTOS configurato correttamente nel tuo sistema. Puoi seguire la guida ufficiale di Zephyr per la configurazione dell'ambiente di sviluppo e del toolchain:

[Guida ufficiale Zephyr - Getting Started](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)

Questa guida ti guiderà nell'installazione di tutti gli strumenti necessari per lavorare con Zephyr, inclusi i prerequisiti come Python, Git, CMake, e il tool **west**.

### 2. Clonare il progetto

Una volta preparato l'ambiente, puoi clonare il progetto nella directory `zephyrproject`. Esegui i seguenti comandi nel tuo terminale:

```bash
cd ~
cd zephyrproject
git clone https://github.com/gmrossetti/pir-detector-esp32.git
cd pir-detector-esp32

```

### 3. Compilare il progetto

Dopo aver clonato il progetto, puoi compilare il firmware per la tua scheda di sviluppo (ad esempio, un ESP32). Utilizza il comando `west` per costruire il progetto, specificando il tipo di scheda da utilizzare con l'opzione `-b` (ad esempio, `esp32`):

```bash
west build -b esp32
```

### 4. Flashare il firmware sulla scheda

Una volta completata la compilazione, puoi caricare il firmware sulla tua scheda di sviluppo utilizzando il comando `west flash`:

```bash
west flash
```

## Wiring dei Componenti

Per il corretto funzionamento del progetto, è necessario collegare i componenti come segue:

### Componenti:
- **Sensore PIR**: Utilizzato per rilevare il movimento.
- **LED 5mm + Resistore**: Utilizzato per visualizzare l'azione (acceso quando viene rilevato un movimento).

### Connessioni:

- **Sensore PIR**:
  - **VCC**: Collegare al pin di alimentazione **esterna** **5V**. Se la scheda ESP32 viene alimentata SOLO tramite USB, è possibile collegarlo al pin **VIN** dell'ESP32, evitando così la necessità di una fonte di alimentazione esterna.
  - **GND**: Collegare al pin **GND** della scheda ESP32 e dell'eventuale alimentatore esterno (massa comune).
  - **OUT**: Collegare al pin **GPIO 25** della scheda ESP32.

- **LED 5mm + Resistore**:
  - **Anodo (positivo)**: Collegare al pin **GPIO 12** della scheda ESP32.
  - **Catodo (negativo)**: Collegare al **GND** della scheda ESP32 tramite un resistore adeguato (limitatore corrente).

### Nota:
- Verifica sempre le connessioni dei pin specifici in base alla tua scheda di sviluppo e assicurati di collegare correttamente i pin **VCC** e **GND** per evitare danni ai componenti.
- **Attenzione**: Collegare il **VCC** del **Sensore PIR** al pin **VIN** dell'ESP32 **solo se** la scheda ESP32 è alimentata tramite USB, altrimenti c'è il rischio di danneggiare il sensore PIR.
- Utilizzare **Resistore** con valore adeguato in base al **LED 5mm** e la corrente massima consentita.

## Funzionamento

- **Sensore PIR**: Rileva il movimento nell'ambiente circostante.
- **LED**: Si accende quando viene rilevato un movimento e rimane acceso per 15 secondi.
- **Interrupt**: L'evento di movimento genera un interrupt che accende il LED.
- **Timer**: Viene utilizzato un timer per spegnere automaticamente il LED dopo 15 secondi.

## Struttura del Codice

Il codice si suddivide nelle seguenti funzioni principali:

1. **movement_detected**: Funzione di callback chiamata quando il sensore PIR rileva un movimento. Aggiorna un timer e accende il LED.
2. **configure_led**: Configura il LED per l'output.
3. **configure_pir_sensor**: Configura il sensore PIR per l'input e abilita l'interrupt.
4. **configure_pir_callback**: Configura il callback per il sensore PIR.
5. **main**: Funzione principale che configura il sistema e gestisce la logica del controllo del LED.

