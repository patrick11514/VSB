import bcrypt from 'bcrypt'
import { LoremIpsum } from 'lorem-ipsum'
import fs from 'node:fs'

const data = fs.readFileSync('data.json', 'utf-8').trim()
const json = JSON.parse(data) as {
    username: string
    surname: string
    town: string
    psc: string
}[]

const emails = ['gmail.com', 'seznam.cz', 'patrick115.eu', 'centrum.cz', 'email.cz', 'vsb.cz']

/**
 * table users
 *   user_id   INTEGER NOT NULL,
 *   username  VARCHAR(255) NOT NULL,
 *   email     VARCHAR(255),
 *   password  VARCHAR(72) NOT NULL,
 *   role      VARCHAR(1) NOT NULL
 */

if (!fs.existsSync('users.sql')) {
    fs.writeFileSync('users.sql', `INSERT INTO users (username, email, password, role) VALUES\n`)

    json.forEach((userData, index) => {
        console.log(`Processing ${index + 1}/${json.length}`)

        const username = userData.username
        const surname = userData.surname
        const password = bcrypt.hashSync(username, 10)
        const email = `${username.toLowerCase()}.${surname.toLowerCase()}@${
            emails[Math.floor(Math.random() * emails.length)]
        }`
        let role: 'H' | 'M'

        //first 30 people are moderators
        if (index < 30) {
            role = 'M'
        } else {
            role = 'H'
        }

        if (index === json.length - 1) {
            fs.appendFileSync('users.sql', `('${username}', '${email}', '${password}', '${role}');\n`)
        } else {
            fs.appendFileSync('users.sql', `('${username}', '${email}', '${password}', '${role}'),\n`)
        }
    })
}
/**
 * table tickets
 * 
 *  ticket_id       INTEGER IDENTITY PRIMARY KEY,
    name            VARCHAR(255) NOT NULL,
    category_id          INTEGER NOT NULL,
    created_at      DATETIME NOT NULL,
    author          INTEGER NOT NULL,
    moderator       INTEGER NOT NULL

    status - 0 = open,
    status - 1 = closed
 */

/**
     * table ticket_categories
     * assing ticket to category with history
     * 
     *     ticket_id               INTEGER NOT NULL,
    updated_at              DATETIME NOT NULL,
    moderator               INTEGER NULL, Who changed the category, null at the start, where category is selected by author
    category_id             INTEGER NOT NULL
     */

/**
 * table messages
 * 
    message_id         INTEGER IDENTITY PRIMARY KEY,
    message            VARCHAR(1024) NOT NULL,
    created_at         DATETIME NOT NULL,
    ticket_id          INTEGER NOT NULL,
    author             INTEGER NOT NULL

 */
if (!fs.existsSync('tickets.sql') || !fs.existsSync('ticket_categories.sql') || !fs.existsSync('messages.sql')) {
    const categoriesMax = 10
    const maxUserId = 1000
    const moderatorMaxId = 30

    fs.writeFileSync('tickets.sql', `INSERT INTO tickets (name, status, created_at, author, moderator) VALUES\n`)
    /**

### Problémy s přihlašováním
1. "Chyba při přihlašování: 'Připojení odmítnuto'"
2. "Nemohu se přihlásit na server - 'Session ID is null'"
3. "Problém s ověřením účtu při přihlašování"
4. "Hlásím problém s loginem: 'Internal Exception'"
5. "Vypisuje 'Failed to login: Invalid session'"
6. "Přihlašovací obrazovka se nezobrazuje správně"
7. "Nedostatek informací při pokusu o přihlášení"
8. "Server nepřijímá přihlašovací údaje"
9. "Problém s připojením: 'Connection Lost'"
10. "Nelze se připojit: 'Unknown Host Exception'"

### Ztráta inventáře
1. "Ztráta všech předmětů po pádu serveru"
2. "Inventář zmizel po restartu serveru"
3. "Všechny položky zmizely po odpojení"
4. "Inventář se neuložil správně"
5. "Chybí mi všechny diamanty z inventáře"
6. "Náhodně zmizely všechny nástroje a zbraně"
7. "Položky zmizely po teleportaci na novou lokaci"
8. "Hráči ztratili inventář po změně pluginu"
9. "Předměty se nevrátily po smrti ve hře"
10. "Inventář se nenahrál správně po přihlášení"

### Technické chyby ve hře
1. "Extrémní lag a zasekávání serveru"
2. "Server padá při pokusu o vstup do Endu"
3. "Náhodné pády serveru po úpravě nastavení"
4. "Lag po příchodu do určité oblasti"
5. "Zpomalení hry po poslední aktualizaci"
6. "Chyba s výkonem serveru při větším počtu hráčů"
7. "Vysoká zátěž serveru při používání konkrétního pluginu"
8. "Stagnace a lag v redstone konstrukcích"
9. "Pády serveru při používání specifických příkazů"
10. "Serverové chyby po náhlé změně hardwaru"

### Návrhy pro nový obsah
1. "Nápad na nový quest pro hráče"
2. "Nové vylepšení pro systém PvP"
3. "Návrh na rozšíření frakcí"
4. "Eventová aktivita pro více hráčů"
5. "Představa nového módu pro hru"
6. "Návrh na speciální bonus pro aktivní hráče"
7. "Nový prvek do ekonomického systému serveru"
8. "Návrhy na tematické eventy"
9. "Inovativní přídavek do systému obchodu"
10. "Vylepšení nových hráčů při startu na serveru"

### Porušení pravidel
1. "Hráč griefuje stavbu na mé parcele"
2. "Nahlášení hráče používajícího hacky"
3. "Nekorektní chování hráče v chatu"
4. "Hráč neustále ignoruje pravidla serveru"
5. "Hráč úmyslně ničí okolí a stavby ostatních"
6. "Porušení pravidel týkající se PvP"
7. "Násilné chování hráče vůči ostatním"
8. "Hráč šíří nevhodný obsah ve veřejném chatu"
9. "Nahlášení hráče pro vyhrožování"
10. "Neadekvátní používání exploitů ve hře"

### Technická podpora
1. "Potřebuji pomoc s nastavením optifine"
2. "Chyba při instalaci shaderů"
3. "Optimalizace grafických nastavení ve hře"
4. "Návod na instalaci modpacku"
5. "Potíže s propojením s Discordem"
6. "Nápověda s konfigurací nového herního zařízení"
7. "Problémy s herním zvukem po aktualizaci"
8. "Návod na správné nastavení klávesnicových zkratek"
9. "Nastavení výkonu hry podle specifikací PC"
10. "Pomoc s aktualizací Java verze pro hru"

### Přidělení pozemku
1. "Žádost o nový pozemek pro budoucí stavbu"
2. "Potřebuji větší prostor pro nový projekt"
3. "Problém s vlastnictvím na parcelách"
4. "Přidělení pozemku pro nového hráče"
5. "Problém s přístupem na vyhrazený pozemek"
6. "Náhodné zmizení stavby na mé parcele"
7. "Problém s rozšířením mého pozemku"
8. "Žádost o pozemek v konkrétní biome"
9. "Potřebuji více prostoru pro rozšíření svého města"
10. "Nahlášení problému s parcelou a stavebním místem"

### Chyby pluginů
1. "Problémy s ekonomickým pluginem"
2. "Plugin pro ochranu pozemků nefunguje správně"
3. "Nelze použít specifický příkaz z pluginu"
4. "Chyba s pluginem pro obchodování"
5. "Plugin pro teleportaci nefunguje správně"
6. "Problémy s pluginem na ochranu inventáře"
7. "Chyba s pluginem pro vytváření skupin"
8. "Náhodné zmizení bloků vytvořených pluginem"
9. "Nelze použít speciální funkci pluginu"
10. "Problém s kompatibilitou pluginu s novou verzí"

### Nábor do týmu
1. "Zájem o roli moderátora na serveru"
2. "Administrátorský tým - zájem o zapojení"
3. "Nabídka zkušeností v moderování na jiném serveru"
4. "Přihláška do týmu event manažerů"
5. "Nábor nových členů do stavebního týmu"
6. "Zájem o pozici průvodce pro nové hráče"
7. "Hledání nových členů pro technickou podporu"
8. "Nábor do týmu pro tvorbu nových eventů"
9. "Hledání kreativních jedinců pro tým"
10. "Zájem o vedení komunity a pomoc hráčům"

### Žádost o změnu jména
1. "Změna herního jména na serveru"
2. "Potřebuji změnit své herní jméno"
3. "Návrhy pro nové herní jméno"
4. "Změna herního jména kvůli osobním důvodům"
5. "Žádost o úpravu herního jména po dlouhé době"
6. "Herní jméno mi přestalo vyhovovat"
7. "Změna jména kvůli novému hernímu stylu"
8. "Žádost o změnu herního jména pro lepší zapadnutí"
9. "Nové jméno pro lepší začlenění do komunity"
10. "Přechod na nové herní jméno pro skupinové aktivity"
     */
    const categoryNames = [
        [
            "Chyba při přihlašování: 'Připojení odmítnuto'",
            "Nemohu se přihlásit na server - 'Session ID is null'",
            'Problém s ověřením účtu při přihlašování',
            "Hlásím problém s loginem: 'Internal Exception'",
            "Vypisuje 'Failed to login: Invalid session'",
            'Přihlašovací obrazovka se nezobrazuje správně',
            'Nedostatek informací při pokusu o přihlášení',
            'Server nepřijímá přihlašovací údaje',
            "Problém s připojením: 'Connection Lost'",
            "Nelze se připojit: 'Unknown Host Exception'",
        ],
        [
            'Ztráta všech předmětů po pádu serveru',
            'Inventář zmizel po restartu serveru',
            'Všechny položky zmizely po odpojení',
            'Inventář se neuložil správně',
            'Chybí mi všechny diamanty z inventáře',
            'Náhodně zmizely všechny nástroje a zbraně',
            'Položky zmizely po teleportaci na novou lokaci',
            'Hráči ztratili inventář po změně pluginu',
            'Předměty se nevrátily po smrti ve hře',
            'Inventář se nenahrál správně po přihlášení',
        ],
        [
            'Extrémní lag a zasekávání serveru',
            'Server padá při pokusu o vstup do Endu',
            'Náhodné pády serveru po úpravě nastavení',
            'Lag po příchodu do určité oblasti',
            'Zpomalení hry po poslední aktualizaci',
            'Chyba s výkonem serveru při větším počtu hráčů',
            'Vysoká zátěž serveru při používání konkrétního pluginu',
            'Stagnace a lag v redstone konstrukcích',
            'Pády serveru při používání specifických příkazů',
            'Serverové chyby po náhlé změně hardwaru',
        ],
        [
            'Nápad na nový quest pro hráče',
            'Nové vylepšení pro systém PvP',
            'Návrh na rozšíření frakcí',
            'Eventová aktivita pro více hráčů',
            'Představa nového módu pro hru',
            'Návrh na speciální bonus pro aktivní hráče',
            'Nový prvek do ekonomického systému serveru',
            'Návrhy na tematické eventy',
            'Inovativní přídavek do systému obchodu',
            'Vylepšení nových hráčů při startu na serveru',
        ],
        [
            'Hráč griefuje stavbu na mé parcele',
            'Nahlášení hráče používajícího hacky',
            'Nekorektní chování hráče v chatu',
            'Hráč neustále ignoruje pravidla serveru',
            'Hráč úmyslně ničí okolí a stavby ostatních',
            'Porušení pravidel týkající se PvP',
            'Násilné chování hráče vůči ostatním',
            'Hráč šíří nevhodný obsah ve veřejném chatu',
            'Nahlášení hráče pro vyhrožování',
            'Neadekvátní používání exploitů ve hře',
        ],
        [
            'Potřebuji pomoc s nastavením optifine',
            'Chyba při instalaci shaderů',
            'Optimalizace grafických nastavení ve hře',
            'Návod na instalaci modpacku',
            'Potíže s propojením s Discordem',
            'Nápověda s konfigurací nového herního zařízení',
            'Problémy s herním zvukem po aktualizaci',
            'Návod na správné nastavení klávesnicových zkratek',
            'Nastavení výkonu hry podle specifikací PC',
            'Pomoc s aktualizací Java verze pro hru',
        ],
        [
            'Žádost o nový pozemek pro budoucí stavbu',
            'Potřebuji větší prostor pro nový projekt',
            'Problém s vlastnictvím na parcelách',
            'Přidělení pozemku pro nového hráče',
            'Problém s přístupem na vyhrazený pozemek',
            'Náhodné zmizení stavby na mé parcele',
            'Problém s rozšířením mého pozemku',
            'Žádost o pozemek v konkrétní biome',
            'Potřebuji více prostoru pro rozšíření svého města',
            'Nahlášení problému s parcelou a stavebním místem',
        ],
        [
            'Problémy s ekonomickým pluginem',
            'Plugin pro ochranu pozemků nefunguje správně',
            'Nelze použít specifický příkaz z pluginu',
            'Chyba s pluginem pro obchodování',
            'Plugin pro teleportaci nefunguje správně',
            'Problémy s pluginem na ochranu inventáře',
            'Chyba s pluginem pro vytváření skupin',
            'Náhodné zmizení bloků vytvořených pluginem',
            'Nelze použít speciální funkci pluginu',
            'Problém s kompatibilitou pluginu s novou verzí',
        ],
        [
            'Zájem o roli moderátora na serveru',
            'Administrátorský tým - zájem o zapojení',
            'Nabídka zkušeností v moderování na jiném serveru',
            'Přihláška do týmu event manažerů',
            'Nábor nových členů do stavebního týmu',
            'Zájem o pozici průvodce pro nové hráče',
            'Hledání nových členů pro technickou podporu',
            'Nábor do týmu pro tvorbu nových eventů',
            'Hledání kreativních jedinců pro tým',
            'Zájem o vedení komunity a pomoc hráčům',
        ],
        [
            'Změna herního jména na serveru',
            'Potřebuji změnit své herní jméno',
            'Návrhy pro nové herní jméno',
            'Změna herního jména kvůli osobním důvodům',
            'Žádost o úpravu herního jména po dlouhé době',
            'Herní jméno mi přestalo vyhovovat',
            'Změna jména kvůli novému hernímu stylu',
            'Žádost o změnu herního jména pro lepší zapadnutí',
            'Nové jméno pro lepší začlenění do komunity',
            'Přechod na nové herní jméno pro skupinové aktivity',
        ],
    ]

    const ticketCategoryPairs: Record<
        number,
        {
            category: number
            date: string
            author: number
            moderator: number
        }
    > = {}

    let id = 1

    categoryNames.forEach((categories, categoryIndex) => {
        categories.forEach((name, index) => {
            const createdAt = new Date()
            const author = Math.floor(Math.random() * maxUserId) + 1
            const moderator = Math.floor(Math.random() * moderatorMaxId) + 1
            const status = Math.floor(Math.random() * 2)

            //random day and month and time
            createdAt.setDate(Math.floor(Math.random() * 30) + 1)
            createdAt.setMonth(Math.floor(Math.random() * 12) + 1)
            createdAt.setHours(Math.floor(Math.random() * 24))
            createdAt.setMinutes(Math.floor(Math.random() * 60))
            createdAt.setSeconds(Math.floor(Math.random() * 60))

            ticketCategoryPairs[id] = {
                category: categoryIndex + 1,
                date: createdAt.toISOString(),
                author,
                moderator,
            }

            if (categoryIndex === categoriesMax - 1 && index === categories.length - 1) {
                fs.appendFileSync(
                    'tickets.sql',
                    `('${name.replaceAll("'", "''")}', ${status}, '${createdAt
                        .toISOString()
                        .replace('T', ' ')}', ${author}, ${moderator});\n`,
                )
            } else {
                fs.appendFileSync(
                    'tickets.sql',
                    `('${name.replaceAll("'", "''")}', ${status}, '${createdAt
                        .toISOString()
                        .replace('T', ' ')}', ${author}, ${moderator}),\n`,
                )
            }

            id++
        })
    })

    fs.writeFileSync(
        'ticket_categories.sql',
        `INSERT INTO ticket_categories (ticket_id, updated_at, moderator, category_id) VALUES\n`,
    )

    Object.entries(ticketCategoryPairs).forEach(([ticketIdS, categoryData]) => {
        const ticketId = parseInt(ticketIdS)
        const categoryId = categoryData.category
        //for each 3.rd ticket generate random number with random prevous random categories changed by random moderator

        if (ticketId % 3 == 0) {
            const random = Math.floor(Math.random() * 8) + 1

            let lastDate: string | undefined

            for (let i = 0; i < random; i++) {
                const updatedAt = new Date()
                const moderator = Math.floor(Math.random() * moderatorMaxId) + 1
                const randomCategoryId = Math.floor(Math.random() * categoriesMax) + 1

                const minDate = new Date(categoryData.date)

                //update to be after the min date and last record
                if (lastDate === undefined) {
                    updatedAt.setDate(minDate.getDate() + 1)
                    updatedAt.setHours(Math.floor(Math.random() * 24))
                    updatedAt.setMinutes(Math.floor(Math.random() * 60))
                    updatedAt.setSeconds(Math.floor(Math.random() * 60))

                    lastDate = updatedAt.toISOString()
                } else {
                    updatedAt.setDate(new Date(lastDate).getDate() + 1)
                    updatedAt.setHours(Math.floor(Math.random() * 24))
                    updatedAt.setMinutes(Math.floor(Math.random() * 60))
                    updatedAt.setSeconds(Math.floor(Math.random() * 60))

                    lastDate = updatedAt.toISOString()
                }

                fs.appendFileSync(
                    'ticket_categories.sql',
                    `(${ticketId}, '${updatedAt.toISOString().replace('T', ' ')}', ${
                        i == 0 ? 'NULL' : moderator
                    }, ${randomCategoryId}),\n`,
                )
            }

            //now insert the last record with correct category
            const updatedAt = new Date()
            const moderator = Math.floor(Math.random() * moderatorMaxId) + 1

            const minDate = new Date(categoryData.date)

            //update to be after the min date and last record
            if (lastDate === undefined) {
                updatedAt.setDate(minDate.getDate() + 1)
                updatedAt.setHours(Math.floor(Math.random() * 24))
                updatedAt.setMinutes(Math.floor(Math.random() * 60))
                updatedAt.setSeconds(Math.floor(Math.random() * 60))

                lastDate = updatedAt.toISOString()
            } else {
                updatedAt.setDate(new Date(lastDate).getDate() + 1)
                updatedAt.setHours(Math.floor(Math.random() * 24))
                updatedAt.setMinutes(Math.floor(Math.random() * 60))
                updatedAt.setSeconds(Math.floor(Math.random() * 60))

                lastDate = updatedAt.toISOString()
            }

            fs.appendFileSync(
                'ticket_categories.sql',
                `(${ticketId}, '${updatedAt.toISOString().replace('T', ' ')}', ${moderator}, ${categoryId}),\n`,
            )
        } else {
            fs.appendFileSync(
                'ticket_categories.sql',
                `(${ticketId}, '${categoryData.date.replace('T', ' ')}', NULL, ${categoryId}),\n`,
            )
        }

        //now insert real category
    })

    //messages
    const maxTicketId = 100

    fs.writeFileSync('messages.sql', `INSERT INTO messages (message, created_at, ticket_id, author) VALUES\n`)

    for (let i = 1; i <= maxTicketId; i++) {
        const messageCount = Math.floor(Math.random() * 15) + 1

        let lastMessageDate: string | undefined

        for (let l = 0; l < messageCount; l++) {
            const text = new LoremIpsum({
                sentencesPerParagraph: {
                    max: 8,
                    min: 4,
                },
                wordsPerSentence: {
                    max: 20,
                    min: 8,
                },
            }).generateParagraphs(2)

            const createdAt = new Date()

            const minDate = new Date(ticketCategoryPairs[i].date)

            if (!lastMessageDate) {
                createdAt.setDate(minDate.getDate() + 1)
                createdAt.setHours(Math.floor(Math.random() * 24))
                createdAt.setMinutes(Math.floor(Math.random() * 60))
                createdAt.setSeconds(Math.floor(Math.random() * 60))

                lastMessageDate = createdAt.toISOString()
            } else {
                createdAt.setDate(new Date(lastMessageDate).getDate() + 1)
                createdAt.setHours(Math.floor(Math.random() * 24))
                createdAt.setMinutes(Math.floor(Math.random() * 60))
                createdAt.setSeconds(Math.floor(Math.random() * 60))

                lastMessageDate = createdAt.toISOString()
            }

            fs.appendFileSync(
                'messages.sql',
                `('${text.replaceAll("'", "''")}', '${createdAt.toISOString().replace('T', ' ')}', ${i}, ${
                    ticketCategoryPairs[i].author
                }),\n`,
            )
        }
    }
}

//merge sqls
const files = ['users.sql', 'tickets.sql', 'ticket_categories.sql', 'messages.sql']

if (!fs.existsSync('merged.sql')) {
    //read all files and before file enter -- =============== FILENAME ===============
    files.forEach((file) => {
        const content = fs.readFileSync(file, 'utf-8')

        //if file ends with ,\n replace it to ;
        if (content.endsWith(',\n')) {
            fs.appendFileSync('merged.sql', `-- ============== ${file} ===============\n${content.slice(0, -2)};\n\n`)
        } else {
            fs.appendFileSync('merged.sql', `-- ============== ${file} ===============\n${content}\n\n`)
        }
    })
}
