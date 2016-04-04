---------------------------- OTServ configuration ------------------------------

-- datadir
datadir = "data/"

-- sets what map to load
map = "data/world/test.otbm"

-- OTBM for binary, SQL for SQL map, XML for OTX maps
mapkind = "OTBM"

-- SQL for acc/player info from SQL, XML for acc/player info from XML
sourcedata = "XML"

-- the message the player gets when he logs in
loginmsg = "Welcome to Divinity"

-- the port otserv listens on
port = "7171"

-- name of our server
servername = "Divinity"

-- name of the owner of our server
ownername = "Gihow"

-- email of the owner of our server
owneremail = "gihow@roxors.net"

-- additional comments
othercomments = "Your comments will show up in !info box"

-- the url for more server info
url = "www.otfans.net"

-- the location of the server
location = "USA"

-- the ip the server should redirect too
ip = "127.0.0.1"

-- The messagebox you sometimes get before you choose characters
motd = "Welcome to Divinity"
motdnum="0"

-- use md5 passwords for accounts, yes/no
md5passwords = "no"

-- world type. options: pvp, no-pvp, pvp-enforced
worldtype = "pvp"

-- max number of players allowed
maxplayers = "30"

-- exhausted time in ms (1000 = 1sec)
exhausted = 1000

-- exhaustion time for healing spells (1000 = 1sec)
exhaustedheal = 1000

-- how many ms to add if the player is already exhausted and tries to cast a spell (1000 = 1sec)
exhaustedadd = 1000

-- how long does the player has to stay out of fight to get pz unlocked in ms (1000 = 1sec)
pzlocked = 60*1000

-- allow multiple logins of the same char
allowclones = 0

-- vocation names
vocations = {"a sorcerer", "a druid", "a paladin", "a knight"}
promoted_vocations = {"a master sorcerer", "an elder druid", "a royal paladin", "an elite knight"}


--- SQL part
sql_host = "localhost"
sql_user = "root"
sql_pass = ""
sql_db   = "otserv"


--- SQL MAP part
sqlmap = "test_map"
map_host = "localhost"
map_user = "root"
map_pass = ""
map_db   = ""

------------------------- Divinity basic configuration ---------------------------

-- name of your world (seen on the character list when logging in)
worldname = "Divinity"

-- how often do server saves it's state (0 = off, 5 = 5min, 60 = hour)
autosave = 10

-- how often do server saves characters  (1 = 1min)
playersave = 9

-- how often do server saves houses (1 = 1min)
housesave = 11

-- how often do server saves guilds  (1 = 1min)
guildsave = 60

-- do you want to enable cap system? (yes/no)
capsystem = "yes"

-- do you want players to learn spells before they can use them? (yes/no)
learnspells = "no"

-- do you want to give premium account to all players? (yes/no)
freepremmy = "no"

-- do you want premium players to wait in queue as others? (yes/no)
queuepremmy = "no"

-- how much % of {exp, mana, skill, eq, bp} do you lose when dying
diepercent = {"7", "7", "7", "7", "100"}

-- how many summons player can have
maxsummons = 2

-- do you want to give summons for all vocations? (yes/no)
summonsallvoc = "no"

-- chance of losing a spear when shot (0 - none lost, 100000 - all lost)
spearlosechance = 50

-- anti-afk - maximum idle time to kick player (1 = 1min)
kicktime = 25

-- maximum number of items player can keep in his/her depot
maxdepotitems = 1000

-- maximum number of items on a house tile (including those in containers)
maxhousetileitems = 1000

-- how much death entries are saved in players file (old are deleted)
maxdeathentries = 10

-- mana consumend by {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodmana = {"2", "3", "5", "8", "13"}

-- mana consumed by wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandmana = {"2", "3", "5", "8", "13"}

-- shooting range of {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodrange = {"4", "3", "2", "1", "3"}

-- shooting range of  wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandrange = {"4", "3", "2", "1", "3"}

-- damage of burst arrows blast
-- default is from (1*lvl+5*mlvl)*0.24 to (1*lvl+5*mlvl)*0.55
burstarrowdmg = {"1.0", "5.0", "0.24", "0.55"}

-- speed of attack in seconds (no-voc, sorc, druid, pally, knight)
speed = {"2.5", "2.0", "2.0", "1.5", "1"}

-- house price for 1 sqm
priceforsqm = 1000

-- max houses for player
maxhouses = 2

---------------- Life Ring and ROH ----
-- Life Ring Configuration
LifeRingHealth = "15"
LifeRingMana = "15"

-- ROH Configuration
RoHHealth = "30"
RoHMana = "30"

--------------------------- Multipliers and gains ------------------------------

-- experience multiplier (how much faster you got exp from monsters)
expmul = 1

-- experience multiplier for pvp-enforced (how much faster you got exp from players)
expmulpvp = 2

-- skill multipliers: 1 - rl tibia, 10 - ten times faster etc. {no-voc, sorcerer, druid, paladin, knight}
weaponmul = {"1", "1", "1", "1", "2"}
distmul = {"1", "1", "1", "2", "1"}
shieldmul = {"1", "1", "1", "2", "2"}
manamul = {"0", "1", "1", "1", "1"}

-- how much points of life,mana,cap do you get when advancing {no-voc, sorcerer, druid, paladin, knight}
capgain = {"10", "10", "10", "20", "25"}
managain = {"5", "30", "30", "15", "5"}
hpgain = {"5", "5", "5", "10", "15"}

-- how much faster do you recovery life,mana after eating food (1 - rl tibia)
healthtickmul = 5
manatickmul = 5

--How many mana and Hp you recovery using Soft Boots
softmana = "6"
softhealth = "6"

-- How many % increase magic damage when u have bluerobe?
bluerobe = 10

-------------------------------- Skull system ----------------------------------

-- how many unjustified kills to get a red skull
redunjust = 3

-- how many unjustified kills to get banned
banunjust = 6

-- how long do you have white skull after attacking player (1 = 1min)
hittime = 2

-- how long do you have white skull after killing player (1 = 1min)
whitetime = 15

-- how long do you have red skull (1 = 1min)
redtime = 6*60

-- how long do you have to wait to lose 1 unjustified kill (1 = 1min)
fragtime = 12*60
------------------------------ GM access rights --------------------------------

-- access to walk into houses and open house doors
accesshouse = 3

-- access to be afk and not be kicked
accessnokick = 3

-- access to login without waiting in the queue or when server is closed
accessenter = 2

-- access to ignore damage, exhaustion, cap limit and be ignored by monsters
accessprotect = 2

-- access to broadcast messages and talk in colors (#c blabla - in public channels)
accesstalk = 1

-- access to move distant items from/to distant locations
accessremote = 3

-- access to see id and position of the item you are looking at
accesslook = 2