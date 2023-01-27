# 2d arpg combat game
Trying to recreate a "secret of mana" style action RPG combat


### notes:
How should enemies behave?

Enemies need to have some kind of idle movement. I.e., walk in a direction for a bit, wait for a bit, repeat.
Enemies need to have a "following player" state
If they touch the player, deal damage to the player... this isn't really a 'state' though.
If the player damages the enemy they need to go into a "hurt" state where they are knocked back and perhaps dazed.
