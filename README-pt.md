# ETS2Sync Helper v4.0+
[ENGLISH VERSION](README.md)

[LICENSE](LICENSE.md)

Este é o repositório para o ETS2Sync Helper 4.0 em diante. O programa foi refeito do zero em C++, o que o tornou menor e mais rápido.

Este programa sincroniza a lista de cargas do Euro Truck Simulator 2, facilitando a organização de comboios no Multiplayer.

No momento, o programa só está disponível em inglês. Pode ser que outros idiomas sejam (re-)adicionados no futuro.

## Download
http://files.dsantosdev.com/ets2sync_helper.zip

## Compatibilidade
Esta versão do ETS2Sync Helper é compatível com o Euro Truck Simulator 1.25, mas apenas para saves criados em formato Texto (`g_save_format 2`).

Se o seu jogo não estiver configurado com `g_save_format 2`, o programa permitirá que você mude essa configuração. Para mudar o formato do save por meio do programa, você precisa estar com o jogo fechado, do contrário a mudança não será reconhecida pelo jogo.

Se você tiver o Console de Desenvolvimento ativado no jogo, você pode usar o comando `g_save_format 2` antes de salvar o jogo para sincronizar, e então você pode usar o comando `g_save_format 0` depois de sincronizar. Dessa forma o jogo não vai dar lag quando criar autosaves, o que pode ocorrer se você dirigir com `g_save_format` configurado para Texto (2). Se você usar o Console de Desenvolvimento para configurar o formato do save, você pode ignorar a mensagem em amareno no programa dizendo que o formato do save recomendado é Texto.

## Clear Jobs
Uma funcionalidade adicionada nesta versão é a possibilidade de esvaziar o Mercado de Fretes. Dessa forma, se você não quer mais jogar com a lista sincronizada e quiser que o jogo gere uma lista de cargas nova para você jogar sozinho, você só precisa fazer o mesmo processo que o de sincronização, mas clicando em “Clear Jobs” em vez de “Sync Jobs”. Ao carregar o save, o Mercado de Fretes vai estar vazio. Aí basta pular para a oficina com o F7 e o jogo gerará novas cargas para você.

## Bugs/Problemas
Se você encontrar um problema no programa, informe no link abaixo:

https://github.com/davidsantos-br/ETS2Sync-Helper-4/issues
