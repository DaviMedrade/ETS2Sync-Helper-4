# ETS2Sync Helper v4.0+
[ENGLISH VERSION](README.md)

[LICENSE](LICENSE.md)

* [Download](#download)
* [Configuração Inicial](#configuração-inicial)
  * [Método Simples](#configuração-inicial-método-simples)
  * [Método Avançado](#configuração-inicial-método-avançado)
    * [Usando o Console](#método-avançado-usando-o-console)
* [Como Sincronizar](#como-sincronizar)
* [Funcionalidades](#funcionalidades)
  * [Clear Jobs](#clear-jobs)
  * [Economy Reset](#economy-reset)
  * [Job List](#job-list)
* [Erros Comuns](#erros-comuns)
* [Compilando o Código-Fonte](#compilando-o-código-fonte)

Este é o repositório para o  ETS2Sync Helper 4.0 e mais novos. O programa foi recriado do zero em C++, o que o tornou menor e mais rápido.

Este programa sincroniza a lista de cargas do Euro Truck Simulator 2 ou American Truck Simulator, tornando mais fácil a organização de comboios no Multiplayer.

No momento o programa só está disponível em inglês. Outros idiomas podem ser (re-)adicionados no futuro.

## Download
http://files.dsantosdev.com/ets2sync_helper.zip

## Configuração Inicial
Antes de começar a sincronizar, você precisa escolher se vai usar o método simples ou o avançado. O método simples é mais fácil, mas ele faz com que o ETS2/ATS trave por um ou dois segundos a cada cinco ou dez minutos, quando o jogo cria um autosave. Isso pode fazer outros jogadores baterem no seu caminhão já que para eles o seu caminhão vai parar de repente e continuar a andar depois de um ou dois segundos. Por esse motivo, eu recomendo o método avançado. É um pouco mais complexo, mas não tem esses lags de autosave. Siga a seção abaixo correspondente ao método que você escolher. Você só vai precisar fazer isso uma vez para cada jogo (i.e. uma vez para o ETS2 e uma vez para o ATS).

### Configuração Inicial: Método Simples
Abra o ETS2Sync Helper e selecione o jogo (ETS2 or ATS). Olhe no quadro “Game Settings”. Se houver uma mensagem em verde dizendo “Settings OK”, já está tudo certo. Se a mensagem for amarela ou vermelha, você precisa fazer o seguinte:
1. Certifique-se que o jogo está fechado. Isso não vai funcionar se for feito com o jogo aberto (mesmo que esteja minimizado).
2. No ETS2Sync Helper, no quadro “Game Settings”, na linha “Save Format”, clique em “Set to ‘Text’”. A mensagem em amarelo ou vermelho deve sumir e em seu lugar aparecer a mensagem “Settings OK” em verde. É isso. Agora o seu jogo está pronto para sincronizar, então você pode ir para [Como Sincronizar](#como-sincronizar).

### Configuração Inicial: Método Avançado
Para usar este método, você precisa abrir o arquivo `config.cfg` na pasta do jogo em Documentos, e mudar `g_developer` para 1 (um), `g_save_format` para 0 (zero), e `g_console` para 1 (um). A primeira e a terceira mudanças ativam o Console de Desenvolvimento—você pode pular essas mudanças se o Console já estiver ativado—e a segunda muda o Formato do Save para Binário—você pode pular essa também se o ETS2Sync Helper estiver mostrando “Save Format: Binary”. Aqui vai um passo-a-passo para fazer essas configurações:

1. Certifique-se que o jogo está fechado. Isso não vai funcionar se for feito com o jogo aberto (mesmo que esteja minimizado).
2. Abra o ETS2Sync Helper. Se você estiver jogando ATS, selecione “ATS” na opção “Game”.
3. No quadro “Game Settings”, há uma linha dizendo “Settings Folder: <pasta de configuração do ETS2/ATS> - Options”. Clique em “Options” → “Open game config file”. Uma janela do Bloco de Notas aparecerá, com o arquivo de configurações do jogo aberto.
4. Nessa janela do Bloco de Notas, vá em “Editar” → “Localizar…” (ou tecle <kbd>Ctrl</kbd>+<kbd>F</kbd>). Na janela de busca, digite `g_developer` (incluindo o underline) e tecle <kbd>Enter</kbd>. Isso deve destacar uma linha no arquivo que diz `uset g_developer "0"`. Se o número não for `1`, troque para `1`, de modo que fique `uset g_developer "1"`.
4. Vá em “Editar” → “Localizar…” (ou tecle <kbd>Ctrl</kbd>+<kbd>F</kbd>) novamente. Na janela de busca, digite `g_save_format` e tecle <kbd>Enter</kbd>. Isso deve destacar uma linha no arquivo que diz `uset g_save_format "2"` ou algum outro número. Se o número não for zero, mude para `0`, de modo que fique `uset g_save_format "0"`.
5. Vá em “Editar” → “Localizar…” (ou tecle <kbd>Ctrl</kbd>+<kbd>F</kbd>) novamente. Na janela de busca, digite `g_console` e tecle <kbd>Enter</kbd>. Isso deve destacar uma linha no arquivo que diz `uset g_console "0"`. Se o número não for `1`, mude para `1`, de modo que fique `uset g_console "1"`.
6. Salve o arquivo e feche o Bloco de Notas. Você pode ignorar a mensagem em amarelo no ETS2Sync Helper.

#### Método Avançado: Usando o Console
Uma vez que o Console esteja ativado (conforme a seção [Configuração Inicial: Método Avançado](#configuração-inicial-método-avançado)), quando você estiver no jogo, você pode abrir e fechar o Console usando a tecla acima do <kbd>Tab</kbd>, à esquerda do <kbd>1</kbd> no teclado alfanumérico.

Quando você precisar executar um comando no Console, abra o Console, digite o comando como estiver escrito (ou copie e tecle <kbd>Ctrl</kbd>+<kbd>V</kbd> no Console para colar), e em seguida tecle <kbd>Enter</kbd>. Se uma mensagem em vermelho dizendo `unknown command` (`comando desconhecido`) aparecer, você provavelmente cometeu um erro ao digitar o comando. Certifique-se de digitá-lo exatamente como escrito: em minúsculas, com underlines e espaços nos locais corretos, e sem confundir `1` (um) com `l` (`L` minúsculo) ou `0` (zero) com `O` (`O` maiúsculo).

Você pode re-digitar um comando que você executou antes usando <kbd>↑</kbd> (seta para cima) e <kbd>↓</kbd> (seta para baixo) para navegar pelo histórico de comandos. Uma vez que tenha encontrado o comando que deseja, você pode fazer mudanças nele se necessário e então teclar <kbd>Enter</kbd> para executar.

Se você digitar o começo de um comando e teclar <kbd>Tab</kbd>, o Console vai autocompletar o resto para você, mas só se não tiver nenhum outro comando que começa da mesma forma. Se houver, o Console vai mostrar uma lista de possíveis comandos, para que você possa digitar mais alguns caracteres para desambiguar antes de teclar <kbd>Tab</kbd> novamente. Se você teclar <kbd>Tab</kbd> e não acontecer nada, isso quer dizer que os caracteres que você digitou não correspondem ao começo de nenhum comando válido. Note que este recurso só funciona para a primeira “palavra” do comando (i.e. a parte antes do primeiro espaço).

## Como Sincronizar
1. Vá para a tela principal do jogo (se estiver dirigindo, tecle <kbd>Esc</kbd>).
2. **Somente Método Avançado:** No Console, execute o comando `g_save_format 2`. Você pode ver a seção [Método Avançado: Usando o Console](#método-avançado-usando-o-console) para aprender como fazer isto.
3. Clique em “Salvar e Carregar” → “Salvar o Jogo”.
4. Digite um nome para o save ou selecione um save existente para salvar por cima dele.
5. Clique em “Salvar”.
6. Tecle `Alt`+`Tab` para minimizar o jogo.
7. Abra o ETS2Sync Helper se já não estiver aberto. Certifique-se que o jogo correto está selecionado na opção “Game” do quadro “Game Settings”.
8. Certifique-se que o perfil e save corretos estão selecionados nos quadros “Select ETS2/ATS Profile” e “Select ETS2/ATS Save”.
9. Selecione as DLCs que você e seus amigos tiverem (não selecione uma DLC a menos que todos com quem você for jogar também tenham).
10. Clique em “Sync Jobs”.
11. Quando a mensagem “Sync Complete” aparecer, clique em “Close”.
12. Volte para o jogo.
13. Clique em “Salvar e Carregar” → “Carregar o Jogo”.
14. Selecione o save que você acabou de sincronizar (o que você criou/substituiu no passo 4).
15. Clique em “Carregar”.
16. **Somente Método Avançado:** No Console, execute o comando `g_save_format 0` (note que é um número zero, e não uma letra “O”).
17. Verifique o Mercado de Fretes. Se todas as cargas tiverem o mesmo tempo para expirar (“A oferta expira em”), e for cerca de 400 a 500 horas, então a sincronização funcionou.

## Funcionalidades
### Clear Jobs
Este recurso oferece a possibilidade de esvaziar o Mercado de Fretes. Dessa forma, se você terminou de usar a lista sincronizada e quer que o jogo gere uma nova lista de cargas para você jogar sozinho, basta fazer o mesmo processo que a sincronização, mas clique em “Clear Jobs” em vez de “Sync Jobs”. Quando você carregar o save, o Mercado de Fretes estará vazio. Basta pular para a oficina com <kbd>F7</kbd> ou fazer uma Viagem Rápida ou duas e o jogo gerará novas cargas para você.

### Economy Reset
Este recurso funciona como o Clear Jobs, mas é mais rápido porque quando você carregar o save o jogo mostrará uma mensagem “Mudança no Jogo Detectada”, que gerará novas cargas automaticamente (i.e. não é necessário pular para a oficina ou fazer Viagens Rápidas para popular o Mercado de Fretes). Uma desvantagem é que o seu caminhão será movido para a sua garagem principal.

### Job List
Quando você sincroniza, o programa faz o download de uma lista de cargas do servidor e insere no seu save. O servidor possui oito listas de cargas para o ETS2 e oito para o ATS. Uma dessas oito listas é a que está ativa naquela semana—a que será usada se você deixar a configuração “Job List” na opção “Automatic”. Toda quarta-feira a lista ativa muda para a próxima (i.e. se a lista ativa é a Lista 7, na quarta-feira passa a ser a Lista 8 e na quarta-feira seguinte volta para a Lista 1).

As listas de cargas em si não mudam, exceto quando necessário devido a uma atualização do jogo. Então, se você precisa que a lista de cargas não mude—por exemplo, se é segunda-feira e você está planejando um comboio para o fim de semana—, basta selecionar manualmente uma das oito listas e informar a todos no comboio que essa lista deve ser selecionada no programa antes de sincronizar.

## Erros Comuns

### Unfortunately, binary saves aren't supported yet. Please create the save using a text format.
**Método Simples**: Você selecionou um save antigo ou o jogo não está configurado corretamente.  
* Se o ETS2Sync Helper não estiver mostrando a mensagem “Settings OK” em verde no quadro “Game Settings”, você precisa seguir os passos da seção [Configuração Inicial: Método Simples](#configuração-inicial-método-simples) novamente.
* Se a mensagem “Settings OK” já estava aparecendo, certifique-se que o perfil e save corretos estão selecionados antes de sincronizar.
* Se ainda assim não funcionar, feche o ETS2Sync Helper e o jogo, e em seguida abra o ETS2Sync Helper novamente e veja se a mensagem “Settings OK” em verde apareceu. Se não tiver aparecido, veja acima. Se tiver, abra o jogo e certifique-se de criar um novo save antes de sincronizar novamente.

**Método Avançado**: O perfil ou save errado está selecionado no ETS2Sync Helper, ou você esqueceu de executar `g_save_format 2` no Console antes de criar o save que você tentou sincronizar.

### Could not download the job list. HTTP 426 Upgrade Required
Você está usando uma versão desatualizada do ETS2Sync Helper. Verifique a página [Releases](https://github.com/davidsantos-br/ETS2Sync-Helper-4/releases) e baixe a versão mais recente. Se isso acontecer mesmo depois de atualizar para a versão mais nova, é porque uma versão nova está prestes a ser lançada. Aguarde alguns minutos e verifique a página Releases novamente.

### Could not download the job list. InternetOpenURL error
### Could not download the job list. HTTP 500 Internal Server Error
### Could not download the job list. HTTP 401 Unauthorized
### Could not download the job list. HTTP 403 Forbidden
### Could not download the job list. HTTP 404 Not Found
Isso é provavelmente um problema temporário com a sua conexão à Internet ou com o servidor de onde o ETS2Sync Helper baixa a lista de cargas. Se a sua conexão estiver funcionando corretamente, aguarde alguns minutos e tente novamente.

### Método Avançado: `unknown command` no Console
Você provavelmente digitou o comando incorretamente. Note que o comando `g_save_format` precisa ser digitado exatamente como escrito—em minúsculas, com underlines—e é necessário um espaço antes do `2` ou `0`.

## Bugs/Problemas
Se você encontrar um problema no programa, informe no link abaixo:

https://github.com/davidsantos-br/ETS2Sync-Helper-4/issues

## Compilando o Código-Fonte
Antes de compilar, você precisa preparar as dependências.

### Dependências
Para as que precisam ser compiladas, instruções para a compilação podem geralmente ser encontradas no arquivo baixado ou na documentação da biblioteca em questão.

* **wxWidgets 3.1** — baixe e compile, e então coloque o diretório onde foi compilada na variável de ambiente `WXWIN`.
   
   wxWidgets fornece várias classes utilitárias usadas no programa, principalmente para interface gráfica e suporte a threads. O código do programa seria bem mais complexo se estas funcionalidades tivessem que ser desenvolvidas usando a API nativa do Windows.
* **zlib** — baixe e compile, e então coloque o diretório onde foi compilada na variável de ambiente `ZLIBDIR`.

   Quando arquivos `.sii` criptografados são criados pelos jogos, os dados são compactados e em seguida criptografados. A zlib é usada para descompactar os dados, depois de serem descriptografados.
* **Ragel 6** — baixe e instale um executável para Windows (`ragel.exe`) ou baixe e compile o código-fonte, e então adicione o diretório do arquivo `ragel.exe` à sua `PATH`.

   O Ragel é usado para gerar o código das state machines que lêem arquivos `.cfg` e arquivos `.sii` em modo texto.
* **GraphViz** — baixe e instale uma versão para Windows ou baixe e compile o código-fonte, e então adicione o diretório do arquivo `dot.exe` à sua `PATH`.

   O GraphViz gera uma representação visual dos parsers gerados pelo Ragel. Isso facilita entender como esses parsers funcionam e consequentemente como corrigir problemas neles se necessário.

### Compilação
Uma vez configuradas as dependências, abra o arquivo `ETS2Sync-Helper-4.sln` no Visual Studio 2017 e compile normalmente. Esteja ciente que numa compilação em modo debug o processo de sincronização é substancialmente mais lento.
