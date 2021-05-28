# mnote - a sticky note CLI

## test environment
Ubuntu-18.04

## install
```bash
cd mnote
sudo make install
```

## uninstall
```bash
cd mnote
sudo make uninstall
```

## usage

### 1. create one new note

Use`mnote -n`, where `n`stands for `new`.

Each note will be assigned with a `index`, which would be used in other commands.

![](./assets/do_new.png)

### 2. open specific note with Vim

Use`mnote -v (index)`, where `v` stands for `Vim`.

![](./assets/do_vim.png)

### 3. list all notes

Use`mnote`or`mnote -l`, where `l` stands for `list`.

![](./assets/do_list.png)

### 4. delete specific note

Use`mnote -d (index)`, where `d` stands for `delete`.

![](./assets/do_delete.png)