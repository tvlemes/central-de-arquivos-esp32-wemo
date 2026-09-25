/*
====================================================================
                       WEB SERVER
====================================================================

Central Dados
WEMOS LOLIN32 / ESP32
MicroSD + Wi-Fi

Funcionalidades:

- Página Web
- Listagem de arquivos
- Leitura de arquivos
- Edição de arquivos
- Salvamento
- Download
- Upload
- Exclusão
- Informações do cartão SD
====================================================================
*/

#include "web_server.h"

#include <Arduino.h>
#include <WebServer.h>
#include <FS.h>
#include <SD.h>

#include "config.h"
#include "sd_manager.h"

// ==================================================================
// SERVIDOR WEB
// ==================================================================

WebServer server(80);

// ==================================================================
// ARQUIVO SENDO ENVIADO
// ==================================================================

File uploadFile;

// ==================================================================
// PÁGINA HTML
// ==================================================================

const char WEB_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html lang="pt-BR">

<head>

<meta charset="UTF-8">

<meta
    name="viewport"
    content="width=device-width, initial-scale=1.0"
>

<title>Central de Dados</title>

<style>

* {
    box-sizing: border-box;
}

body {
    margin: 0;
    padding: 0;

    font-family:
        Arial,
        Helvetica,
        sans-serif;

    background: #0f172a;
    color: #e5e7eb;
}

.container {
    width: 95%;
    max-width: 1200px;

    margin: 0 auto;
}

header {
    background: #111827;

    padding: 25px 20px;

    border-bottom:
        1px solid #334155;

    margin-bottom: 20px;
}

header h1 {
    margin: 0;

    font-size: 28px;
}

header p {
    margin:
        8px 0 0 0;

    color: #94a3b8;
}

.card {
    background: #1e293b;

    border:
        1px solid #334155;

    border-radius: 12px;

    padding: 20px;

    margin-bottom: 20px;
}

.stats {
    display: grid;

    grid-template-columns:
        repeat(3, 1fr);

    gap: 15px;

    margin-bottom: 20px;
}

.stat {
    background: #111827;

    border:
        1px solid #334155;

    border-radius: 10px;

    padding: 18px;

    text-align: center;
}

.stat-title {
    color: #94a3b8;

    font-size: 14px;

    margin-bottom: 8px;
}

.stat-value {
    font-size: 22px;

    font-weight: bold;
}

h2 {
    margin-top: 0;

    font-size: 20px;
}

button {
    border: 0;

    border-radius: 6px;

    padding:
        9px 14px;

    cursor: pointer;

    color: white;

    font-size: 14px;

    margin: 3px;
}

.btn-primary {
    background: #2563eb;
}

.btn-primary:hover {
    background: #1d4ed8;
}

.btn-danger {
    background: #dc2626;
}

.btn-danger:hover {
    background: #b91c1c;
}

.btn-secondary {
    background: #475569;
}

.btn-secondary:hover {
    background: #334155;
}

.btn-success {
    background: #16a34a;
}

.btn-success:hover {
    background: #15803d;
}

input[type="file"] {
    width: 100%;

    background: #020617;

    border:
        1px solid #334155;

    color: #e5e7eb;

    padding: 10px;

    border-radius: 6px;

    margin-bottom: 10px;
}

table {
    width: 100%;

    border-collapse: collapse;
}

th,
td {
    padding: 12px;

    text-align: left;

    border-bottom:
        1px solid #334155;
}

th {
    color: #94a3b8;
}

td {
    word-break: break-word;
}

textarea {
    width: 100%;

    min-height: 350px;

    resize: vertical;

    background: #020617;

    color: #e5e7eb;

    border:
        1px solid #334155;

    border-radius: 8px;

    padding: 15px;

    font-family:
        Consolas,
        monospace;

    font-size: 14px;
}

#editor {
    display: none;
}

#editorTitle {
    color: #60a5fa;
}

.status {
    padding: 12px;

    margin-top: 15px;

    border-radius: 6px;

    background: #111827;

    border:
        1px solid #334155;

    color: #94a3b8;
}

.actions {
    white-space: nowrap;
}

.progress {
    width: 100%;

    height: 8px;

    background: #020617;

    border-radius: 5px;

    overflow: hidden;

    margin-top: 10px;

    display: none;
}

.progress-bar {
    height: 100%;

    width: 0%;

    background: #2563eb;
}

@media (max-width: 700px) {

    .stats {
        grid-template-columns: 1fr;
    }

    th:nth-child(2),
    td:nth-child(2) {
        display: none;
    }

    .actions {
        white-space: normal;
    }

    button {
        margin:
            2px 0;
    }
}

</style>

</head>

<body>

<header>

<div class="container">

<h1>💾 Central Dados</h1>

<p>
WEMOS LOLIN32 • ESP32 • MicroSD • Wi-Fi
</p>

</div>

</header>

<div class="container">

<!-- ============================================================
     INFORMAÇÕES DO SD
     ============================================================ -->

<div class="stats">

<div class="stat">

<div class="stat-title">
Total
</div>

<div
    class="stat-value"
    id="total"
>
--
</div>

</div>

<div class="stat">

<div class="stat-title">
Utilizado
</div>

<div
    class="stat-value"
    id="used"
>
--
</div>

</div>

<div class="stat">

<div class="stat-title">
Livre
</div>

<div
    class="stat-value"
    id="free"
>
--
</div>

</div>

</div>

<!-- ============================================================
     ARQUIVOS
     ============================================================ -->

<div class="card">

<h2>📁 Arquivos</h2>

<button
    class="btn-primary"
    onclick="loadFiles()"
>
🔄 Atualizar
</button>

<div style="overflow-x:auto; margin-top:15px;">

<table>

<thead>

<tr>

<th>
Arquivo
</th>

<th>
Tamanho
</th>

<th>
Tipo
</th>

<th>
Ações
</th>

</tr>

</thead>

<tbody id="fileList">

<tr>

<td colspan="4">
Carregando...
</td>

</tr>

</tbody>

</table>

</div>

</div>

<!-- ============================================================
     UPLOAD
     ============================================================ -->

<div class="card">

<h2>📤 Enviar arquivo</h2>

<input
    type="file"
    id="fileInput"
>

<button
    class="btn-primary"
    onclick="uploadFile()"
>
📤 Enviar
</button>

<div
    class="progress"
    id="progress"
>

<div
    class="progress-bar"
    id="progressBar"
></div>

</div>

</div>

<!-- ============================================================
     EDITOR
     ============================================================ -->

<div
    class="card"
    id="editor"
>

<h2>
📝 Editar:
<span id="editorTitle"></span>
</h2>

<textarea id="editorContent"></textarea>

<br>

<button
    class="btn-success"
    onclick="saveFile()"
>
💾 Salvar
</button>

<button
    class="btn-secondary"
    onclick="closeEditor()"
>
✖ Fechar
</button>

</div>

<!-- ============================================================
     STATUS
     ============================================================ -->

<div
    class="status"
    id="status"
>
Pronto.
</div>

</div>

<script>

let currentFile = "";

// ================================================================
// STATUS
// ================================================================

function showStatus(message)
{
    document.getElementById("status").innerText =
        message;
}

// ================================================================
// TAMANHO
// ================================================================

function formatBytes(bytes)
{
    if (bytes < 1024)
        return bytes + " B";

    if (bytes < 1024 * 1024)
        return (
            (bytes / 1024).toFixed(2)
            + " KB"
        );

    if (bytes < 1024 * 1024 * 1024)
        return (
            (bytes / 1024 / 1024).toFixed(2)
            + " MB"
        );

    return (
        (bytes / 1024 / 1024 / 1024).toFixed(2)
        + " GB"
    );
}

// ================================================================
// INFORMAÇÕES SD
// ================================================================

async function loadInfo()
{
    try
    {
        const response =
            await fetch("/api/info");

        const data =
            await response.json();

        if (data.success)
        {
            document.getElementById("total")
                .innerText =
                formatBytes(data.total);

            document.getElementById("used")
                .innerText =
                formatBytes(data.used);

            document.getElementById("free")
                .innerText =
                formatBytes(data.free);
        }
        else
        {
            showStatus(
                "Erro ao obter informações do SD."
            );
        }
    }
    catch (error)
    {
        showStatus(
            "Erro de comunicação com o ESP32."
        );
    }
}

// ================================================================
// LISTAR ARQUIVOS
// ================================================================

async function loadFiles()
{
    showStatus(
        "Carregando arquivos..."
    );

    try
    {
        const response =
            await fetch("/api/files");

        const data =
            await response.json();

        const list =
            document.getElementById(
                "fileList"
            );

        list.innerHTML = "";

        if (!data.success)
        {
            list.innerHTML =
                "<tr><td colspan='4'>" +
                "Erro ao listar arquivos." +
                "</td></tr>";

            return;
        }

        if (data.files.length === 0)
        {
            list.innerHTML =
                "<tr><td colspan='4'>" +
                "Nenhum arquivo encontrado." +
                "</td></tr>";

            showStatus(
                "Nenhum arquivo encontrado."
            );

            return;
        }

        data.files.forEach(function(file)
        {
            const row =
                document.createElement("tr");

            const fileName =
                file.name;

            const path =
                file.path;

            row.innerHTML =

                "<td>" +
                escapeHtml(fileName) +
                "</td>" +

                "<td>" +
                formatBytes(file.size) +
                "</td>" +

                "<td>" +
                escapeHtml(file.type) +
                "</td>" +

                "<td class='actions'>" +

                "<button " +
                "class='btn-primary' " +
                "onclick=\"openFile('" +
                encodeURIComponent(path) +
                "')\">" +
                "📖 Abrir" +
                "</button>" +

                "<button " +
                "class='btn-secondary' " +
                "onclick=\"downloadFile('" +
                encodeURIComponent(path) +
                "')\">" +
                "⬇ Baixar" +
                "</button>" +

                "<button " +
                "class='btn-danger' " +
                "onclick=\"deleteFile('" +
                encodeURIComponent(path) +
                "')\">" +
                "🗑 Excluir" +
                "</button>" +

                "</td>";

            list.appendChild(row);
        });

        showStatus(
            "Arquivos atualizados."
        );
    }
    catch (error)
    {
        console.error(error);

        showStatus(
            "Erro ao carregar arquivos."
        );
    }
}

// ================================================================
// ESCAPAR HTML
// ================================================================

function escapeHtml(text)
{
    return text
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}

// ================================================================
// ABRIR ARQUIVO
// ================================================================

async function openFile(encodedPath)
{
    const path =
        decodeURIComponent(encodedPath);

    showStatus(
        "Abrindo arquivo..."
    );

    try
    {
        const response =
            await fetch(
                "/api/read?path=" +
                encodeURIComponent(path)
            );

        const data =
            await response.json();

        if (!data.success)
        {
            showStatus(
                data.message ||
                "Erro ao abrir arquivo."
            );

            return;
        }

        currentFile =
            path;

        document.getElementById(
            "editorTitle"
        ).innerText =
            path;

        document.getElementById(
            "editorContent"
        ).value =
            data.content;

        document.getElementById(
            "editor"
        ).style.display =
            "block";

        window.scrollTo(
            0,
            document.getElementById(
                "editor"
            ).offsetTop
        );

        showStatus(
            "Arquivo aberto."
        );
    }
    catch (error)
    {
        console.error(error);

        showStatus(
            "Erro ao abrir arquivo."
        );
    }
}

// ================================================================
// SALVAR
// ================================================================

async function saveFile()
{
    if (!currentFile)
    {
        showStatus(
            "Nenhum arquivo selecionado."
        );

        return;
    }

    const content =
        document.getElementById(
            "editorContent"
        ).value;

    showStatus(
        "Salvando arquivo..."
    );

    try
    {
        const body =
            "path=" +
            encodeURIComponent(currentFile) +
            "&content=" +
            encodeURIComponent(content);

        const response =
            await fetch(
                "/api/save",
                {
                    method: "POST",

                    headers:
                    {
                        "Content-Type":
                            "application/x-www-form-urlencoded"
                    },

                    body: body
                }
            );

        const data =
            await response.json();

        if (data.success)
        {
            showStatus(
                "Arquivo salvo com sucesso."
            );

            loadFiles();
            loadInfo();
        }
        else
        {
            showStatus(
                data.message ||
                "Erro ao salvar arquivo."
            );
        }
    }
    catch (error)
    {
        console.error(error);

        showStatus(
            "Erro ao salvar arquivo."
        );
    }
}

// ================================================================
// FECHAR EDITOR
// ================================================================

function closeEditor()
{
    document.getElementById(
        "editor"
    ).style.display =
        "none";

    currentFile = "";
}

// ================================================================
// DOWNLOAD
// ================================================================

function downloadFile(encodedPath)
{
    const path =
        decodeURIComponent(encodedPath);

    window.location.href =
        "/api/download?path=" +
        encodeURIComponent(path);
}

// ================================================================
// EXCLUIR
// ================================================================

async function deleteFile(encodedPath)
{
    const path =
        decodeURIComponent(encodedPath);

    if (!confirm(
        "Deseja realmente excluir:\n\n" +
        path +
        "?"
    ))
    {
        return;
    }

    showStatus(
        "Excluindo arquivo..."
    );

    try
    {
        const response =
            await fetch(
                "/api/delete",
                {
                    method: "POST",

                    headers:
                    {
                        "Content-Type":
                            "application/x-www-form-urlencoded"
                    },

                    body:
                        "path=" +
                        encodeURIComponent(path)
                }
            );

        const data =
            await response.json();

        if (data.success)
        {
            showStatus(
                "Arquivo excluído com sucesso."
            );

            loadFiles();
            loadInfo();
        }
        else
        {
            showStatus(
                data.message ||
                "Erro ao excluir arquivo."
            );
        }
    }
    catch (error)
    {
        console.error(error);

        showStatus(
            "Erro ao excluir arquivo."
        );
    }
}

// ================================================================
// UPLOAD
// ================================================================

function uploadFile()
{
    const input =
        document.getElementById(
            "fileInput"
        );

    if (!input.files.length)
    {
        showStatus(
            "Selecione um arquivo."
        );

        return;
    }

    const file =
        input.files[0];

    const formData =
        new FormData();

    formData.append(
        "file",
        file
    );

    const xhr =
        new XMLHttpRequest();

    const progress =
        document.getElementById(
            "progress"
        );

    const progressBar =
        document.getElementById(
            "progressBar"
        );

    progress.style.display =
        "block";

    progressBar.style.width =
        "0%";

    showStatus(
        "Enviando arquivo..."
    );

    xhr.upload.onprogress =
        function(event)
        {
            if (event.lengthComputable)
            {
                const percent =
                    (
                        event.loaded /
                        event.total
                    ) * 100;

                progressBar.style.width =
                    percent + "%";
            }
        };

    xhr.onload =
        function()
        {
            progress.style.display =
                "none";

            try
            {
                const data =
                    JSON.parse(
                        xhr.responseText
                    );

                if (xhr.status === 200 &&
                    data.success)
                {
                    showStatus(
                        "Arquivo enviado com sucesso."
                    );

                    input.value = "";

                    loadFiles();
                    loadInfo();
                }
                else
                {
                    showStatus(
                        data.message ||
                        "Erro no upload."
                    );
                }
            }
            catch (error)
            {
                showStatus(
                    "Resposta inválida do ESP32."
                );
            }
        };

    xhr.onerror =
        function()
        {
            progress.style.display =
                "none";

            showStatus(
                "Erro de comunicação durante upload."
            );
        };

    xhr.open(
        "POST",
        "/api/upload"
    );

    xhr.send(formData);
}

// ================================================================
// INICIALIZAÇÃO
// ================================================================

window.onload =
    function()
    {
        loadInfo();
        loadFiles();
    };

</script>

</body>

</html>

)rawliteral";

// ==================================================================
// NORMALIZAR CAMINHO
// ==================================================================

String normalizePath(String path)
{
    path.trim();

    if (path.length() == 0)
    {
        return "/";
    }

    if (!path.startsWith("/"))
    {
        path = "/" + path;
    }

    return path;
}

// ==================================================================
// VALIDAR CAMINHO
// ==================================================================

bool isSafePath(String path)
{
    path = normalizePath(path);

    if (!path.startsWith("/"))
    {
        return false;
    }

    if (path.indexOf("..") >= 0)
    {
        return false;
    }

    if (path.indexOf("\\") >= 0)
    {
        return false;
    }

    return true;
}

// ==================================================================
// NOME DO ARQUIVO
// ==================================================================

String getFileName(String path)
{
    path = normalizePath(path);

    int position =
        path.lastIndexOf('/');

    if (position >= 0)
    {
        return path.substring(
            position + 1
        );
    }

    return path;
}

// ==================================================================
// TIPO DO ARQUIVO
// ==================================================================

String getContentType(String path)
{
    String lowerPath =
        path;

    lowerPath.toLowerCase();

    if (lowerPath.endsWith(".html") ||
        lowerPath.endsWith(".htm"))
    {
        return "text/html";
    }

    if (lowerPath.endsWith(".css"))
    {
        return "text/css";
    }

    if (lowerPath.endsWith(".js"))
    {
        return "application/javascript";
    }

    if (lowerPath.endsWith(".json"))
    {
        return "application/json";
    }

    if (lowerPath.endsWith(".txt"))
    {
        return "text/plain";
    }

    if (lowerPath.endsWith(".csv"))
    {
        return "text/csv";
    }

    if (lowerPath.endsWith(".xml"))
    {
        return "application/xml";
    }

    if (lowerPath.endsWith(".pdf"))
    {
        return "application/pdf";
    }

    if (lowerPath.endsWith(".jpg") ||
        lowerPath.endsWith(".jpeg"))
    {
        return "image/jpeg";
    }

    if (lowerPath.endsWith(".png"))
    {
        return "image/png";
    }

    if (lowerPath.endsWith(".gif"))
    {
        return "image/gif";
    }

    if (lowerPath.endsWith(".ico"))
    {
        return "image/x-icon";
    }

    if (lowerPath.endsWith(".mp3"))
    {
        return "audio/mpeg";
    }

    if (lowerPath.endsWith(".wav"))
    {
        return "audio/wav";
    }

    if (lowerPath.endsWith(".mp4"))
    {
        return "video/mp4";
    }

    return "application/octet-stream";
}

// ==================================================================
// PÁGINA PRINCIPAL
// ==================================================================

void handleRoot()
{
    server.send_P(
        200,
        "text/html; charset=utf-8",
        WEB_PAGE
    );
}

// ==================================================================
// LISTAR ARQUIVOS
// ==================================================================

void handleListFiles()
{
    if (!sdInitialized)
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"SD não inicializado.\"}"
        );

        return;
    }

    String json =
        "{\"success\":true,\"files\":[";

    File root =
        SD.open("/");

    if (!root ||
        !root.isDirectory())
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"Não foi possível abrir o diretório raiz.\"}"
        );

        return;
    }

    bool first =
        true;

    File file =
        root.openNextFile();

    while (file)
    {
        String name =
            file.name();

        if (!first)
        {
            json += ",";
        }

        first = false;

        String path =
            name;

        if (!path.startsWith("/"))
        {
            path =
                "/" + path;
        }

        String type =
            file.isDirectory()
            ? "Diretório"
            : getContentType(path);

        json += "{";

        json +=
            "\"name\":\"" +
            String(name) +
            "\",";

        json +=
            "\"path\":\"" +
            String(path) +
            "\",";

        json +=
            "\"size\":" +
            String(
                file.isDirectory()
                ? 0
                : file.size()
            ) +
            ",";

        json +=
            "\"type\":\"" +
            type +
            "\"";

        json += "}";

        file.close();

        file =
            root.openNextFile();
    }

    root.close();

    json += "]}";

    server.send(
        200,
        "application/json",
        json
    );
}

// ==================================================================
// LER ARQUIVO
// ==================================================================

void handleReadFile()
{
    if (!sdInitialized)
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"SD não inicializado.\"}"
        );

        return;
    }

    if (!server.hasArg("path"))
    {
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Caminho não informado.\"}"
        );

        return;
    }

    String path =
        normalizePath(
            server.arg("path")
        );

    if (!isSafePath(path))
    {
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Caminho inválido.\"}"
        );

        return;
    }

    if (path == "/")
    {
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Não é possível abrir a raiz.\"}"
        );

        return;
    }

    if (!SD.exists(path))
    {
        server.send(
            404,
            "application/json",
            "{\"success\":false,\"message\":\"Arquivo não encontrado.\"}"
        );

        return;
    }

    File file =
        SD.open(path, FILE_READ);

    if (!file)
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"Não foi possível abrir o arquivo.\"}"
        );

        return;
    }

    if (file.isDirectory())
    {
        file.close();

        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"O caminho é um diretório.\"}"
        );

        return;
    }

    String content;

    while (file.available())
    {
        content +=
            (char)file.read();
    }

    file.close();

    // Escapar JSON
    content.replace(
        "\\",
        "\\\\"
    );

    content.replace(
        "\"",
        "\\\""
    );

    content.replace(
        "\r",
        "\\r"
    );

    content.replace(
        "\n",
        "\\n"
    );

    content.replace(
        "\t",
        "\\t"
    );

    String json =
        "{\"success\":true,\"content\":\"" +
        content +
        "\"}";

    server.send(
        200,
        "application/json",
        json
    );
}

// ==================================================================
// SALVAR ARQUIVO
// ==================================================================

void handleSaveFile()
{
    if (!sdInitialized)
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"SD não inicializado.\"}"
        );

        return;
    }

    if (!server.hasArg("path") ||
        !server.hasArg("content"))
    {
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Caminho ou conteúdo não informado.\"}"
        );

        return;
    }

    String path =
        normalizePath(
            server.arg("path")
        );

    String content =
        server.arg("content");

    if (!isSafePath(path))
    {
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Caminho inválido.\"}"
        );

        return;
    }

    if (path == "/")
    {
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Não é possível salvar na raiz.\"}"
        );

        return;
    }

    // --------------------------------------------------------------
    // Se já existe, verificar se é arquivo
    // --------------------------------------------------------------

    if (SD.exists(path))
    {
        File oldFile =
            SD.open(path, FILE_READ);

        if (oldFile)
        {
            if (oldFile.isDirectory())
            {
                oldFile.close();

                server.send(
                    400,
                    "application/json",
                    "{\"success\":false,\"message\":\"O caminho é um diretório.\"}"
                );

                return;
            }

            oldFile.close();
        }

        if (!SD.remove(path))
        {
            server.send(
                500,
                "application/json",
                "{\"success\":false,\"message\":\"Não foi possível substituir o arquivo existente.\"}"
            );

            return;
        }
    }

    // --------------------------------------------------------------
    // Criar novo arquivo
    // --------------------------------------------------------------

    File file =
        SD.open(
            path,
            FILE_WRITE
        );

    if (!file)
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"Não foi possível criar o arquivo.\"}"
        );

        return;
    }

    size_t written =
        file.print(content);

    file.flush();

    file.close();

    if (written != content.length())
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"Nem todo o conteúdo foi gravado.\"}"
        );

        return;
    }

    server.send(
        200,
        "application/json",
        "{\"success\":true,\"message\":\"Arquivo salvo com sucesso.\"}"
    );
}

// ==================================================================
// DOWNLOAD
// ==================================================================

void handleDownload()
{
    if (!sdInitialized)
    {
        server.send(
            500,
            "text/plain",
            "SD não inicializado."
        );

        return;
    }

    if (!server.hasArg("path"))
    {
        server.send(
            400,
            "text/plain",
            "Caminho não informado."
        );

        return;
    }

    String path =
        normalizePath(
            server.arg("path")
        );

    if (!isSafePath(path))
    {
        server.send(
            400,
            "text/plain",
            "Caminho inválido."
        );

        return;
    }

    if (!SD.exists(path))
    {
        server.send(
            404,
            "text/plain",
            "Arquivo não encontrado."
        );

        return;
    }

    File file =
        SD.open(
            path,
            FILE_READ
        );

    if (!file)
    {
        server.send(
            500,
            "text/plain",
            "Não foi possível abrir o arquivo."
        );

        return;
    }

    if (file.isDirectory())
    {
        file.close();

        server.send(
            400,
            "text/plain",
            "O caminho é um diretório."
        );

        return;
    }

    String fileName =
        getFileName(path);

    server.sendHeader(
        "Content-Disposition",
        "attachment; filename=\"" +
        fileName +
        "\""
    );

    server.streamFile(
        file,
        "application/octet-stream"
    );

    file.close();
}

// ==================================================================
// UPLOAD
// ==================================================================

void handleUpload()
{
    if (!sdInitialized)
    {
        Serial.println(
            "UPLOAD: SD não inicializado."
        );

        return;
    }

    HTTPUpload& upload =
        server.upload();

    if (upload.status ==
        UPLOAD_FILE_START)
    {
        String path =
            normalizePath(
                upload.filename
            );

        Serial.println();
        Serial.println(
            "========================================"
        );

        Serial.println(
            "UPLOAD INICIADO"
        );

        Serial.print(
            "Arquivo: "
        );

        Serial.println(path);

        if (!isSafePath(path) ||
            path == "/")
        {
            Serial.println(
                "Caminho de upload inválido."
            );

            return;
        }

        if (uploadFile)
        {
            uploadFile.close();
        }

        if (SD.exists(path))
        {
            Serial.println(
                "Arquivo existente. Removendo..."
            );

            if (!SD.remove(path))
            {
                Serial.println(
                    "ERRO ao remover arquivo existente."
                );

                return;
            }
        }

        uploadFile =
            SD.open(
                path,
                FILE_WRITE
            );

        if (!uploadFile)
        {
            Serial.println(
                "ERRO ao criar arquivo."
            );

            return;
        }

        Serial.println(
            "Arquivo criado."
        );
    }

    else if (
        upload.status ==
        UPLOAD_FILE_WRITE
    )
    {
        if (uploadFile)
        {
            size_t written =
                uploadFile.write(
                    upload.buf,
                    upload.currentSize
                );

            if (written !=
                upload.currentSize)
            {
                Serial.println(
                    "ERRO ao gravar bloco do upload."
                );
            }
        }
    }

    else if (
        upload.status ==
        UPLOAD_FILE_END
    )
    {
        if (uploadFile)
        {
            uploadFile.flush();
            uploadFile.close();
        }

        Serial.print(
            "Upload concluído. Bytes: "
        );

        Serial.println(
            upload.totalSize
        );

        Serial.println(
            "========================================"
        );
    }

    else if (
        upload.status ==
        UPLOAD_FILE_ABORTED
    )
    {
        Serial.println(
            "UPLOAD CANCELADO."
        );

        if (uploadFile)
        {
            uploadFile.close();
        }
    }
}

// ==================================================================
// FINALIZAÇÃO DO UPLOAD
// ==================================================================

void handleUploadComplete()
{
    server.send(
        200,
        "application/json",
        "{\"success\":true,\"message\":\"Upload concluído com sucesso.\"}"
    );
}

// ==================================================================
// EXCLUIR ARQUIVO
// ==================================================================

void handleDeleteFile()
{
    Serial.println();
    Serial.println(
        "========================================"
    );

    Serial.println(
        "             EXCLUSÃO"
    );

    Serial.println(
        "========================================"
    );

    // --------------------------------------------------------------
    // SD
    // --------------------------------------------------------------

    if (!sdInitialized)
    {
        Serial.println(
            "ERRO: SD não inicializado."
        );

        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"SD não inicializado.\"}"
        );

        return;
    }

    // --------------------------------------------------------------
    // PATH
    // --------------------------------------------------------------

    if (!server.hasArg("path"))
    {
        Serial.println(
            "ERRO: path não informado."
        );

        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Caminho não informado.\"}"
        );

        return;
    }

    String path =
        server.arg("path");

    Serial.print(
        "Path recebido: "
    );

    Serial.println(path);

    // --------------------------------------------------------------
    // NORMALIZAR
    // --------------------------------------------------------------

    path =
        normalizePath(path);

    Serial.print(
        "Path normalizado: "
    );

    Serial.println(path);

    // --------------------------------------------------------------
    // VALIDAR
    // --------------------------------------------------------------

    if (!isSafePath(path))
    {
        Serial.println(
            "ERRO: caminho inválido."
        );

        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Caminho inválido.\"}"
        );

        return;
    }

    // --------------------------------------------------------------
    // IMPEDIR EXCLUSÃO DA RAIZ
    // --------------------------------------------------------------

    if (path == "/")
    {
        Serial.println(
            "ERRO: tentativa de excluir raiz."
        );

        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"A raiz do SD não pode ser excluída.\"}"
        );

        return;
    }

    // --------------------------------------------------------------
    // VERIFICAR EXISTÊNCIA
    // --------------------------------------------------------------

    if (!SD.exists(path))
    {
        Serial.println(
            "ERRO: arquivo não existe."
        );

        server.send(
            404,
            "application/json",
            "{\"success\":false,\"message\":\"Arquivo não encontrado.\"}"
        );

        return;
    }

    // --------------------------------------------------------------
    // ABRIR PARA VERIFICAR
    // --------------------------------------------------------------

    File file =
        SD.open(
            path,
            FILE_READ
        );

    if (!file)
    {
        Serial.println(
            "ERRO: não foi possível abrir."
        );

        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"Não foi possível abrir o arquivo.\"}"
        );

        return;
    }

    bool directory =
        file.isDirectory();

    file.close();

    if (directory)
    {
        Serial.println(
            "ERRO: diretórios não são excluídos."
        );

        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"O caminho é um diretório.\"}"
        );

        return;
    }

    // --------------------------------------------------------------
    // EXCLUIR
    // --------------------------------------------------------------

    Serial.println(
        "Chamando SD.remove()..."
    );

    delay(50);

    bool removed =
        SD.remove(path);

    delay(100);

    if (!removed)
    {
        Serial.println(
            "SD.remove() retornou FALSE."
        );

        // ----------------------------------------------------------
        // SEGUNDA VERIFICAÇÃO
        // ----------------------------------------------------------

        if (!SD.exists(path))
        {
            Serial.println(
                "Arquivo não existe mais."
            );

            server.send(
                200,
                "application/json",
                "{\"success\":true,\"message\":\"Arquivo excluído com sucesso.\"}"
            );

            return;
        }

        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"O cartão SD recusou a exclusão do arquivo.\"}"
        );

        return;
    }

    // --------------------------------------------------------------
    // VERIFICAR EXCLUSÃO
    // --------------------------------------------------------------

    if (SD.exists(path))
    {
        Serial.println(
            "ERRO: arquivo ainda existe."
        );

        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"A exclusão foi executada, mas o arquivo ainda existe.\"}"
        );

        return;
    }

    Serial.println(
        "Arquivo excluído com sucesso!"
    );

    server.send(
        200,
        "application/json",
        "{\"success\":true,\"message\":\"Arquivo excluído com sucesso.\"}"
    );
}

// ==================================================================
// INFORMAÇÕES DO SD
// ==================================================================

void handleSDInfo()
{
    if (!sdInitialized)
    {
        server.send(
            500,
            "application/json",
            "{\"success\":false,\"message\":\"SD não inicializado.\"}"
        );

        return;
    }

    uint64_t total =
        SD.totalBytes();

    uint64_t used =
        SD.usedBytes();

    uint64_t freeSpace =
        total > used
        ? total - used
        : 0;

    String json =
        "{\"success\":true,";

    json +=
        "\"total\":" +
        String(total) +
        ",";

    json +=
        "\"used\":" +
        String(used) +
        ",";

    json +=
        "\"free\":" +
        String(freeSpace);

    json += "}";

    server.send(
        200,
        "application/json",
        json
    );
}

// ==================================================================
// SERVIDOR WEB
// ==================================================================

void setupWebServer()
{
    Serial.println();
    Serial.println(
        "========================================"
    );

    Serial.println(
        "          INICIANDO SERVIDOR WEB"
    );

    Serial.println(
        "========================================"
    );

    // --------------------------------------------------------------
    // PÁGINA PRINCIPAL
    // --------------------------------------------------------------

    server.on(
        "/",
        HTTP_GET,
        handleRoot
    );

    // --------------------------------------------------------------
    // LISTAR
    // --------------------------------------------------------------

    server.on(
        "/api/files",
        HTTP_GET,
        handleListFiles
    );

    // --------------------------------------------------------------
    // LER
    // --------------------------------------------------------------

    server.on(
        "/api/read",
        HTTP_GET,
        handleReadFile
    );

    // --------------------------------------------------------------
    // SALVAR
    // --------------------------------------------------------------

    server.on(
        "/api/save",
        HTTP_POST,
        handleSaveFile
    );

    // --------------------------------------------------------------
    // DOWNLOAD
    // --------------------------------------------------------------

    server.on(
        "/api/download",
        HTTP_GET,
        handleDownload
    );

    // --------------------------------------------------------------
    // EXCLUIR
    // --------------------------------------------------------------

    server.on(
        "/api/delete",
        HTTP_ANY,
        handleDeleteFile
    );

    // --------------------------------------------------------------
    // INFORMAÇÕES
    // --------------------------------------------------------------

    server.on(
        "/api/info",
        HTTP_GET,
        handleSDInfo
    );

    // --------------------------------------------------------------
    // UPLOAD
    // --------------------------------------------------------------

    server.on(
        "/api/upload",
        HTTP_POST,
        handleUploadComplete,
        handleUpload
    );

    // --------------------------------------------------------------
    // ROTA NÃO ENCONTRADA
    // --------------------------------------------------------------

    server.onNotFound(
        []()
        {
            server.send(
                404,
                "text/plain",
                "404 - Página não encontrada"
            );
        }
    );

    // --------------------------------------------------------------
    // INICIAR SERVIDOR
    // --------------------------------------------------------------

    server.begin();

    Serial.println(
        "Servidor Web iniciado!"
    );

    Serial.print(
        "Acesse: http://"
    );

    if (WiFi.getMode() == WIFI_AP)
    {
        Serial.print(
            WiFi.softAPIP()
        );
    }
    else
    {
        Serial.print(
            WiFi.localIP()
        );
    }

    Serial.println("/");
}

// ==================================================================
// PROCESSAR REQUISIÇÕES
// ==================================================================

void handleWebServer()
{
    server.handleClient();
}