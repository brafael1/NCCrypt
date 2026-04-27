# NCCrypt - Ferramenta de Criptografia

Programa interativo de criptografia e descriptografia em linha de comando para Linux.

## Compilação

```bash
make
```

## Execução

```bash
./nccrypt
```

## Requisitos

- GCC
- Biblioteca ncurses (libncurses-dev no Debian/Ubuntu)

## Instalação da biblioteca ncurses

```bash
# Debian/Ubuntu
sudo apt-get install libncurses-dev

# Fedora/RHEL
sudo dnf install ncurses-devel

# Arch Linux
sudo pacman -S ncurses
```

## Uso

1. Use as SETAS para navegar no menu
2. Pressione ENTER para selecionar
3. Pressione ESC para voltar

### Algoritmos disponíveis

- **Cifra de César**: Deslocamento configurável (1-25)
- **Cifra de Vigenère**: Chave alfabética
- **Substituição Simples**: 26 letras únicos
- **Atbash**: Inversão do alfabeto
- **Base64**: Codificação padrão

## Limpar build

```bash
make clean
```

## Estrutura do projeto

```
NCCrypt/
├── Makefile
├── crypto.h / crypto.c   # Algoritmos de criptografia
├── ui.h    / ui.c       # Interface ncurses
└── main.c               # Ponto de entrada
```
