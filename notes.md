### notes:
How should enemies behave?

Enemies need to have some kind of idle movement. I.e., walk in a direction for a bit, wait for a bit, repeat.
Enemies need to have a "following player" state
If they touch the player, deal damage to the player... this isn't really a 'state' though.
If the player damages the enemy they need to go into a "hurt" state where they are knocked back and perhaps dazed.

So...
- Idle state: walk around, or just wait.
- Move state: move towards player.
- Hurt state: pushed away from player, invulnerble for a few frames, then wait a moment. Transition back to Move state.
- Dead state: needed if there is some death animation or somesuch thing.

Default to Idle state. Check if player is in range at fixed intervals, if player IS in range, switch state to Move state.