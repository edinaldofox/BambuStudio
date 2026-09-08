# Builds sem dependências globais

O BambuStudio mantém as receitas de dependências em `deps/`. Cada plataforma
deve compilar o seu próprio prefixo local; não copie DLLs, `.so` ou frameworks
entre sistemas operacionais ou arquiteturas.

## Pré-requisitos do ambiente de compilação

Estes pacotes são necessários apenas na máquina que compila o aplicativo. As
bibliotecas produzidas em `deps/` (Boost, TBB, CURL e outras) continuam locais
ao build e devem ser incluídas no pacote final; o usuário final não deve
instalá-las manualmente.

### Linux (Ubuntu/Debian)

Instale o compilador, ferramentas CMake e os headers do SDK gráfico/desktop:

```sh
sudo apt-get update
sudo apt-get install \
  build-essential cmake ninja-build git pkgconf extra-cmake-modules nasm \
  libgl1-mesa-dev libglu1-mesa-dev libcairo2-dev \
  libgtk2.0-dev libgtk-3-dev libwebkit2gtk-4.1-dev \
  libdbus-1-dev libudev-dev libwayland-dev wayland-protocols libxkbcommon-dev \
  libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
  libsecret-1-dev libsoup2.4-dev libssl-dev
```

`libglu1-mesa-dev` é indispensável para o header `GL/glu.h`, usado pelo
renderizador OpenGL. O preset atual constrói wxWidgets com GTK2, portanto
`libgtk2.0-dev` e `libcairo2-dev` também são necessários; GTK3 permanece
necessário para componentes modernos da interface. A lista é derivada do
ambiente de build do projeto em `Dockerfile`; distribuições Fedora, Arch e
outras devem instalar os pacotes de desenvolvimento equivalentes.

Antes de configurar o aplicativo, valide os componentes que normalmente
causam uma interrupção tardia no CMake:

```sh
pkg-config --modversion webkit2gtk-4.1 || pkg-config --modversion webkit2gtk-4.0
pkg-config --modversion libavcodec libswscale libavutil
pkg-config --modversion gstreamer-1.0 gstreamer-base-1.0
wx-config --version
```

Se nenhum pacote WebKitGTK for encontrado, no Ubuntu/Debian instale
`libwebkit2gtk-4.1-dev`. Em versões mais antigas da distribuição, use o pacote
de desenvolvimento que fornece `webkit2gtk-4.0`; o projeto aceita ambas as
APIs. O `wx-config` exibido deve ser o do prefixo local após a configuração,
por exemplo `build/deps-linux/destdir/usr/local/bin/wx-config`.
Se a verificação do GStreamer falhar, instale `libgstreamer1.0-dev` e
`libgstreamer-plugins-base1.0-dev`.

### Windows

Instale Visual Studio 2022 com o workload **Desktop development with C++**, o
Windows SDK, Git, CMake 3.20+ e Ninja. Abra o *Developer PowerShell for VS
2022* antes de usar os presets `windows-x64-*`.

### macOS

Instale Xcode e suas ferramentas de linha de comando, CMake 3.20+, Ninja e
Git. Compile ARM64 e x86_64 em builds separados; não reutilize o diretório de
dependências entre arquiteturas.

## 1. Compilar dependências

Execute a partir de `deps/` e escolha o preset da plataforma:

```sh
cmake --preset linux-release
cmake --build --preset linux-release
```

No Windows use `windows-x64-release`. No macOS use `macos-arm64-release` ou
`macos-x86_64-release`. Os artefatos ficam em
`build/deps-<plataforma>/destdir/usr/local`.

## 2. Compilar o aplicativo

Configure o diretório raiz usando o preset correspondente. Ele aponta
`CMAKE_PREFIX_PATH` para o prefixo criado na etapa anterior e habilita o modo
estático quando suportado pela plataforma:

```sh
cmake --preset linux-release-local-deps
cmake --build --preset linux-release-local-deps
```

No Windows e macOS, use os presets `windows-x64-release-local-deps`,
`macos-arm64-release-local-deps` ou `macos-x86_64-release-local-deps`. O
instalador/AppImage/`.app` deve ser produzido na mesma plataforma que será
distribuída.

## Distribuição

- Linux: distribuir AppImage, Flatpak ou pacote que inclua as bibliotecas
  necessárias.
- Windows: incluir DLLs no instalador/diretório do executável.
- macOS: incluir frameworks e bibliotecas em `BambuStudio.app` e assinar o
  bundle após a cópia.

Isso deixa a máquina do usuário final independente de Boost, TBB e CURL.
Os SDKs do sistema listados acima ainda são exigidos apenas para compilar e
empacotar o aplicativo; eles não devem ser uma dependência manual do usuário
final.

### Dependências de execução e pacote autocontido

O artefato distribuído deve ser validado em uma instalação limpa. Não entregue
o diretório de build: ele depende do prefixo local em `build/deps-*` e pode
conter referências ao ambiente da máquina que compilou o aplicativo.

Para os modos FFF e SLA, o pacote deve conter as bibliotecas produzidas em
`deps/` e as dependências transitivas de `libslic3r`, incluindo Boost, TBB,
CURL, OpenSSL, OpenVDB, CGAL e NanoSVG. Assim, importar um modelo, gerar
suportes, hollowing, rasterizar e exportar um arquivo SLA não exige que o
usuário instale SDKs ou bibliotecas de desenvolvimento.

No Linux, as dependências de interface que normalmente permanecem do sistema
são GTK, OpenGL/Mesa, WebKitGTK e D-Bus. A funcionalidade `bambu:///` e a
visualização de vídeo requerem o **runtime** do GStreamer; os pacotes `-dev`
do GStreamer são exclusivamente para compilação. AppImage e Flatpak devem
incluir ou declarar essas dependências de runtime conforme a política do
formato de distribuição. Um pacote `.deb` ou `.rpm` deve listá-las como
dependências de instalação.

Antes de publicar, execute esta lista de validação no artefato instalado:

1. Inicie o aplicativo sem `CMAKE_PREFIX_PATH` nem variáveis de build.
2. Selecione um perfil FFF, fatie e exporte G-code.
3. Selecione um perfil SLA compatível, material e processo SLA; importe um
   modelo, gere suportes/hollowing quando aplicável, fatie e exporte o arquivo
   SLA.
4. Teste a troca FFF ↔ SLA e a reabertura de um projeto de cada tecnologia.
5. Se o produto oferecer integração Bambu, valide descoberta, streaming e
   visualização de vídeo em uma instalação que tenha apenas as dependências de
   runtime declaradas pelo pacote.

Windows deve incluir DLLs ao lado do executável ou no instalador. No macOS,
inclua frameworks e bibliotecas dentro de `BambuStudio.app`, corrija os caminhos
de carregamento e assine/notarize o bundle após essa cópia.
