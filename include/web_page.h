/*
====================================================================
                         WEB PAGE
====================================================================
*/

#ifndef WEB_PAGE_H
#define WEB_PAGE_H


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

    font-family:
        Arial,
        Helvetica,
        sans-serif;

    background:
        #0f172a;

    color:
        #e5e7eb;
}


header {

    padding: 20px;

    background:
        #111827;

    border-bottom:
        1px solid #334155;
}


header h1 {

    margin: 0;

    font-size: 24px;
}


header p {

    margin: 6px 0 0;

    color:
        #94a3b8;
}


.container {

    max-width:
        1200px;

    margin:
        auto;

    padding:
        20px;
}


.cards {

    display:
        grid;

    grid-template-columns:
        repeat(
            auto-fit,
            minmax(
                180px,
                1fr
            )
        );

    gap:
        15px;

    margin-bottom:
        20px;
}


.card {

    background:
        #1e293b;

    border:
        1px solid #334155;

    border-radius:
        12px;

    padding:
        18px;
}


.card-title {

    color:
        #94a3b8;

    font-size:
        13px;
}


.card-value {

    margin-top:
        8px;

    font-size:
        22px;

    font-weight:
        bold;
}


.panel {

    background:
        #1e293b;

    border:
        1px solid #334155;

    border-radius:
        12px;

    overflow:
        hidden;
}


.toolbar {

    display:
        flex;

    gap:
        10px;

    flex-wrap:
        wrap;

    padding:
        15px;

    border-bottom:
        1px solid #334155;
}


button {

    border:
        0;

    border-radius:
        8px;

    padding:
        10px 14px;

    background:
        #2563eb;

    color:
        white;

    cursor:
        pointer;
}


button:hover {

    background:
        #1d4ed8;
}


button.danger {

    background:
        #dc2626;
}


button.secondary {

    background:
        #475569;
}


table {

    width:
        100%;

    border-collapse:
        collapse;
}


th,
td {

    padding:
        13px;

    text-align:
        left;

    border-bottom:
        1px solid #334155;
}


th {

    color:
        #94a3b8;

    font-size:
        13px;
}


tr:hover {

    background:
        #273449;
}


.editor {

    padding:
        15px;
}


textarea {

    width:
        100%;

    min-height:
        400px;

    resize:
        vertical;

    padding:
        15px;

    border:
        1px solid #475569;

    border-radius:
        8px;

    background:
        #020617;

    color:
        #e5e7eb;

    font-family:
        Consolas,
        monospace;

    font-size:
        14px;
}


.upload {

    padding:
        15px;

    border-bottom:
        1px solid #334155;
}


input[type="file"] {

    color:
        #cbd5e1;
}


#status {

    padding:
        10px 15px;

    color:
        #94a3b8;
}


@media(max-width:700px) {

    th:nth-child(2),
    td:nth-child(2) {

        display:
            none;
    }

}

</style>

</head>


<body>


<header>

    <h1>💾 Central de Dados</h1>

    <p>
        WEMOS LOLIN32 • ESP32 • MicroSD • Wi-Fi
    </p>

</header>


<div class="container">


    <div class="cards">


        <div class="card">

            <div class="card-title">
                Armazenamento
            </div>

            <div
                class="card-value"
                id="total"
            >
                --
            </div>

        </div>


        <div class="card">

            <div class="card-title">
                Utilizado
            </div>

            <div
                class="card-value"
                id="used"
            >
                --
            </div>

        </div>


        <div class="card">

            <div class="card-title">
                Livre
            </div>

            <div
                class="card-value"
                id="free"
            >
                --
            </div>

        </div>


    </div>


    <div class="panel">


        <div class="toolbar">

            <button
                onclick="loadFiles()"
            >
                🔄 Atualizar
            </button>

        </div>


        <div class="upload">

            <input
                type="file"
                id="fileInput"
            >

            <button
                onclick="uploadFile()"
            >
                ⬆️ Enviar para SD
            </button>

        </div>


        <div id="status">

            Pronto.

        </div>


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

            </tbody>

        </table>


        <div
            class="editor"
            id="editor"
            style="display:none;"
        >

            <h3 id="editorTitle">
                Editor
            </h3>


            <textarea
                id="editorContent"
            ></textarea>


            <br><br>


            <button
                onclick="saveFile()"
            >
                💾 Salvar
            </button>


            <button
                class="secondary"
                onclick="closeEditor()"
            >
                Fechar
            </button>

        </div>


    </div>

</div>


<script>


// ================================================================
// ARQUIVO ATUAL
// ================================================================

let currentFile = "";


// ================================================================
// FORMATA BYTES
// ================================================================

function formatBytes(bytes)
{
    if (bytes === 0)
    {
        return "0 B";
    }


    const units =
    [
        "B",
        "KB",
        "MB",
        "GB"
    ];


    const index =
        Math.floor(
            Math.log(bytes) /
            Math.log(1024)
        );


    return (
        bytes /
        Math.pow(1024, index)
    ).toFixed(2)
    + " "
    + units[index];
}


// ================================================================
// STATUS
// ================================================================

function setStatus(message)
{
    document.getElementById(
        "status"
    ).innerText = message;
}


// ================================================================
// INFORMAÇÕES
// ================================================================

async function loadInfo()
{
    try
    {
        const response =
            await fetch(
                "/api/info"
            );


        if (!response.ok)
        {
            throw new Error();
        }


        const data =
            await response.json();


        document.getElementById(
            "total"
        ).innerText =
            formatBytes(
                data.total
            );


        document.getElementById(
            "used"
        ).innerText =
            formatBytes(
                data.used
            );


        document.getElementById(
            "free"
        ).innerText =
            formatBytes(
                data.free
            );
    }
    catch(error)
    {
        setStatus(
            "Erro ao carregar informações."
        );
    }
}


// ================================================================
// CARREGA ARQUIVOS
// ================================================================

async function loadFiles()
{
    setStatus(
        "Carregando arquivos..."
    );


    try
    {
        const response =
            await fetch(
                "/api/files"
            );


        if (!response.ok)
        {
            throw new Error();
        }


        const files =
            await response.json();


        const list =
            document.getElementById(
                "fileList"
            );


        list.innerHTML = "";


        files.forEach(
            file =>
            {
                const row =
                    document.createElement(
                        "tr"
                    );


                const name =
                    document.createElement(
                        "td"
                    );


                name.innerText =
                    file.name;


                const size =
                    document.createElement(
                        "td"
                    );


                size.innerText =
                    file.directory
                    ? "-"
                    : formatBytes(
                        file.size
                    );


                const type =
                    document.createElement(
                        "td"
                    );


                type.innerText =
                    file.directory
                    ? "Pasta"
                    : "Arquivo";


                const actions =
                    document.createElement(
                        "td"
                    );


                if (!file.directory)
                {
                    const openButton =
                        document.createElement(
                            "button"
                        );


                    openButton.innerText =
                        "👁️";


                    openButton.onclick =
                        function()
                        {
                            openFile(
                                file.name
                            );
                        };


                    const downloadButton =
                        document.createElement(
                            "button"
                        );


                    downloadButton.innerText =
                        "⬇️";


                    downloadButton.onclick =
                        function()
                        {
                            downloadFile(
                                file.name
                            );
                        };


                    const deleteButton =
                        document.createElement(
                            "button"
                        );


                    deleteButton.innerText =
                        "🗑️";


                    deleteButton.className =
                        "danger";


                    deleteButton.onclick =
                        function()
                        {
                            deleteFile(
                                file.name
                            );
                        };


                    actions.appendChild(
                        openButton
                    );


                    actions.appendChild(
                        document.createTextNode(
                            " "
                        )
                    );


                    actions.appendChild(
                        downloadButton
                    );


                    actions.appendChild(
                        document.createTextNode(
                            " "
                        )
                    );


                    actions.appendChild(
                        deleteButton
                    );
                }


                row.appendChild(name);

                row.appendChild(size);

                row.appendChild(type);

                row.appendChild(actions);


                list.appendChild(row);
            }
        );


        setStatus(
            files.length +
            " item(ns) encontrado(s)."
        );
    }
    catch(error)
    {
        console.error(error);


        setStatus(
            "Erro ao carregar arquivos."
        );
    }
}


// ================================================================
// ABRIR
// ================================================================

async function openFile(path)
{
    setStatus(
        "Abrindo arquivo..."
    );


    try
    {
        const response =
            await fetch(
                "/api/read?path=" +
                encodeURIComponent(path)
            );


        if (!response.ok)
        {
            throw new Error();
        }


        const content =
            await response.text();


        currentFile =
            path;


        document.getElementById(
            "editorTitle"
        ).innerText =
            "Editando: " + path;


        document.getElementById(
            "editorContent"
        ).value =
            content;


        document.getElementById(
            "editor"
        ).style.display =
            "block";


        setStatus(
            "Arquivo carregado."
        );
    }
    catch(error)
    {
        setStatus(
            "Não foi possível abrir o arquivo."
        );
    }
}


// ================================================================
// SALVAR
// ================================================================

async function saveFile()
{
    const content =
        document.getElementById(
            "editorContent"
        ).value;


    const body =
        new URLSearchParams();


    body.append(
        "path",
        currentFile
    );


    body.append(
        "content",
        content
    );


    setStatus(
        "Salvando..."
    );


    try
    {
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


        if (!response.ok)
        {
            throw new Error(
                data.message ||
                "Erro ao salvar."
            );
        }


        setStatus(
            "Arquivo salvo com sucesso."
        );


        await loadFiles();

        await loadInfo();
    }
    catch(error)
    {
        console.error(error);


        setStatus(
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

function downloadFile(path)
{
    window.location.href =
        "/api/download?path=" +
        encodeURIComponent(path);
}


// ================================================================
// EXCLUIR
// ================================================================

async function deleteFile(path)
{
    if (
        !confirm(
            "Deseja realmente excluir " +
            path +
            "?"
        )
    )
    {
        return;
    }


    setStatus(
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


        let data = null;


        try
        {
            data =
                await response.json();
        }
        catch(error)
        {
            data = null;
        }


        console.log(
            "DELETE RESPONSE:",
            response.status,
            data
        );


        if (!response.ok)
        {
            throw new Error(
                data &&
                data.message
                ? data.message
                : "Erro ao excluir."
            );
        }


        setStatus(
            data &&
            data.message
            ? data.message
            : "Arquivo excluído com sucesso."
        );


        await loadFiles();

        await loadInfo();
    }
    catch(error)
    {
        console.error(
            "ERRO DELETE:",
            error
        );


        setStatus(
            error.message ||
            "Erro ao excluir arquivo."
        );
    }
}


// ================================================================
// UPLOAD
// ================================================================

async function uploadFile()
{
    const input =
        document.getElementById(
            "fileInput"
        );


    if (
        input.files.length === 0
    )
    {
        alert(
            "Selecione um arquivo."
        );


        return;
    }


    const formData =
        new FormData();


    formData.append(
        "file",
        input.files[0]
    );


    setStatus(
        "Enviando arquivo..."
    );


    try
    {
        const response =
            await fetch(
                "/api/upload",
                {
                    method: "POST",

                    body: formData
                }
            );


        if (!response.ok)
        {
            throw new Error();
        }


        setStatus(
            "Upload concluído."
        );


        input.value = "";


        await loadFiles();

        await loadInfo();
    }
    catch(error)
    {
        console.error(error);


        setStatus(
            "Erro durante o upload."
        );
    }
}


// ================================================================
// INICIALIZA
// ================================================================

window.addEventListener(
    "load",
    function()
    {
        loadFiles();

        loadInfo();
    }
);


</script>


</body>

</html>

)rawliteral";


#endif