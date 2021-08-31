<p align="center">
  <img src="https://raw.githubusercontent.com/marceloboeira/stonks/master/Stonks.jpg" width="250">
  <h3 align="center">stonks</h3>
  <p align="center">A stock display for Arduino</p>
</p>

## Commands

Available commands with `make COMMAND`:

```
help        List the available commands
compile     Compile to the TARGET_BOARD
setup       Install Arduino CLI
upload      Upload code to the TARGET_BOARD and TARGET_PORT
```
** Important: `upload` command pushes the last compiled code (with `compile` command), not the current code version

### TODO

* [x] Configure LDC Shield
* [x] Configure Internet Shield
* [x] Make random HTTP request
* [x] Plot HTTP response in the LCD
* [x] Control States with a State Machine
* [x] ~How to store list of stocks?~
* [x] ~How to swtich displaying stocks?~
* [x] ~How to update list of stocks?~
* [x] Setup Makefile / Arduino CLI
* [x] Setup editorconfig
* [ ] How to store secrets
* [ ] Find a good/free stock API
* [ ] Implement Request/JSON Response Parser
* [ ] Parse a single stock to display
* [ ] Load Tickers List Dynamically?
* [ ] Use Keyboard to paginate?
* [ ] Abstractions
