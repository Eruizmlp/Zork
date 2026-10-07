# Escape the Office

A small text adventure written in C++ and inspired by the original Zork.

> It's Friday, 13:40. Your shift doesn't end until 17:00, but you've decided to leave early.
> There's just one problem: you're not carrying your motorbike keys.
> Find them and get out of the building before lunch ends at 14:00, without your boss catching you.

Your coworkers walk around the office on their own routes. Some of them will help you; others will
tell the boss everything they see.

**Author:** Eduardo Ruiz · **License:** [MIT](LICENSE)

## Getting started

1. Go to the [Releases](https://github.com/Eruizmlp/Zork/releases) page and download `Zork.zip`.
2. Extract it and run `Zork.exe`.
3. Type `look` to see where you are and `help` to see all the commands.

### Example

```
> look

--- Office ---
Rows of desks and humming monitors. Some coworkers smile at you; others watch you a little too closely.

marc is here.

You see: backpack.

Exits:
  south: A corridor leads south to reception.

> inventory
You are not carrying anything.

Your watch says 13:40. Lunch ends at 14:00 (20 minutes left).
```

## How to play

Every action takes one minute, so you have 20 actions before lunch is over.
Looking around, checking your inventory and talking to people are free.

### Commands

| Command | What it does |
|---|---|
| `go north` or `n` (also `s`, `e`, `w`) | Walk to the next room |
| `look` or `l` | Describe the room, the people in it, the items and the exits |
| `inventory` or `i` | Show what you carry and what time it is |
| `talk marta` or `talk to marta` | Ask someone for information |
| `take <item>` | Pick up an item from the room |
| `take <item> from <container>` | Take an item out of a container |
| `drop <item>` | Leave an item in the room |
| `put <item> in <container>` | Put an item inside a container |
| `give <item> to <person>` | Give an item to someone |
| `wait` or `z` | Let a minute pass (only inside the bathroom) |
| `help` | List all the commands |
| `quit` or `q` | Give up |

### Things worth knowing

- The glass doors at the entrance only open if you hold your **badge** in your hand.
- You need your **motorbike keys** to ride away. They can be in your hand or inside something you carry.
- Whatever you carry in your hand, everyone can see. Whatever is inside a container, nobody can.
- If the **boss** sees you, the game is over. He can never resist a good cigar, though.
- **Snitches** tell the boss when they see something suspicious in your hands. The boss then goes to
  guard the entrance for a few minutes.
- Nobody can see you inside the **bathroom**, and from there you can hear who is in the reception.
  But if someone needs the bathroom and you don't come out, they'll end up telling the boss.

## Walkthrough

<details>
<summary>Show the solution (spoilers!)</summary>

```
take backpack               your badge is inside it, and Marc doesn't care about backpacks
s                           go to the reception
w                           hide in the bathroom
take keys from jacket       leave the jacket hanging: carrying it would give you away
put keys in backpack        keys in your hand are suspicious, inside the backpack nobody sees them
z
z
z                           wait until the boss finishes his smoking break at the entrance
e                           back to the reception
s                           to the entrance
take badge from backpack    the glass doors only open with the badge in your hand
s                           through the glass doors: you made it!
```

Talking to Marta in the reception tells you where your jacket is, where the boss is and that he
takes a smoking break at the entrance. She also hands you a box of cigars a courier left for the boss.

Other strategies:

- Leave the cigars somewhere on the boss's route (for example, in the office): when he finds them,
  he goes outside to smoke one. The building is yours for a while... but don't walk out of the door
  while he is smoking right in front of it.
- Bribe a snitch with something sweet.
- Bring Pablo a coffee from the cafeteria and he will give you some very useful advice.

</details>

## Design

The class structure follows the one proposed in the assignment, with some changes:

```
Entity                 name, description, what it contains and where it is
├── Room               exits, items and creatures inside it
├── Exit               one-way passage with a direction and an optional key
├── Item               some items are containers
└── Creature           knows which room it is in
    ├── Player         the actions typed by the user
    └── NPC            follows a fixed route, with virtual reactions
        ├── Boss       catches the player; guards the entrance, searches the bathroom or goes outside to smoke
        ├── Snitch     tells the boss when he sees a suspicious item; can be bribed
        └── Friendly   gives tips about the boss; some hand you an item or trade a secret for one

Parser                 turns the text typed by the player into a Command
World                  owns every entity, builds the map, runs the turns and decides the ending
```

Main decisions:

- **Everything in the game is an `Entity`, and `Entity::moveTo()` is the only way to move anything.**
  Rooms, exits, items and characters are all entities, and every entity has a list of the entities it
  contains. So "the keys are in the jacket", "the jacket is in the bathroom" and "the player is in the
  reception" are all the same relationship. Moving something is a simple operation: remove it from
  the list of its old container, add it to the list of the new one and update its location. Taking an
  item, dropping it, putting it in the backpack or walking to another room all use this same method,
  so the lists and the locations can never get out of sync.
- **`World` owns every entity** and deletes them in its destructor. Every other pointer only observes.
  `World` and `Entity` can't be copied, so nothing is ever deleted twice.
- **`std::list`** stores what each entity contains, because things are added and removed all the time.
  **`std::vector`** stores all the entities in `World`, because it's iterated every turn and almost never changes.
- **The `Parser` knows nothing about the world**, and `World` never reads raw text: it only receives
  `Command`s.
- **NPC behaviour uses polymorphism instead of type checks:** `World` calls `onPlayerSpotted()`, `talk()`
  or `receiveItem()` on every NPC, and each subclass reacts in its own way.
- **The boss is a small state machine** (`BossState`: calm, guarding the entrance, searching the bathroom, smoking outside).
  A bribed snitch is still a `Snitch` with a flag, because an object can't change its class.
- **Only successful actions take time.** Looking, talking or typing something wrong never costs a minute.

### How a limitation became a mechanic

When I first wrote `Entity::contains()`, it only looked at the entities directly inside, not inside
the containers those entities hold. I hadn't written a search that goes into containers yet, so a
snitch checking `player->contains(keys)` couldn't see the keys if they were inside the backpack.

Instead of "fixing" it, I turned it into a rule of the game: **whatever you carry in your hand,
everyone can see; whatever is inside a container, nobody can.** That simple limitation is what gives
the backpack, `put` and `take ... from` a real purpose: you have to decide what to hide and when to
take it out, like the badge, which only works in your hand.
## What I struggled with

- Keeping an entity's location and the contents of its container in sync, until all movement went through `moveTo()`.
- Balancing the NPC routes so the game can't be won in six commands, but still has a way out within 20 minutes.
- Deciding when a difference between NPCs deserved its own class (the boss) and when it was just data
  (a snitch's suspicious items).

## Building from source

1. Clone the repository.
2. Open `Zork/Zork.sln` with Visual Studio 2019 Community (platform toolset v142, C++17).
3. Select the **Release** configuration and build the solution.
