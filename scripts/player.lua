texture = "images/enemy.png"
speed = 480.0
damage = 10
max_health = 50
detect_range = 5.0
collider_radius = 2.0
anim = {
    walk_south = {
        { x=0,  y=0, w=16, h=32, duration=3 },
        { x=16, y=0, w=16, h=32, duration=3 },
        { x=32, y=0, w=16, h=32, duration=3 },
        { x=48, y=0, w=16, h=32, duration=3 }
    },
    walk_east = {
        { x=0,  y=32, w=16, h=32, duration=3 },
        { x=16, y=32, w=16, h=32, duration=3 },
        { x=32, y=32, w=16, h=32, duration=3 },
        { x=48, y=32, w=16, h=32, duration=3 }
    },
    walk_north = {
        { x=0,  y=64, w=16, h=32, duration=3 },
        { x=16, y=64, w=16, h=32, duration=3 },
        { x=32, y=64, w=16, h=32, duration=3 },
        { x=48, y=64, w=16, h=32, duration=3 }
    },
    walk_west = {
        { x=0,  y=96, w=16, h=32, duration=3 },
        { x=16, y=96, w=16, h=32, duration=3 },
        { x=32, y=96, w=16, h=32, duration=3 },
        { x=48, y=96, w=16, h=32, duration=3 }
    },
    attack_south = {
        { x=0,  y=128, w=32, h=32, duration=3 },
        { x=32, y=128, w=32, h=32, duration=3 },
        { x=64, y=128, w=32, h=32, duration=3 },
        { x=96, y=128, w=32, h=32, duration=3 }
    },
    attack_north = {
        { x=0,  y=160, w=32, h=32, duration=3 },
        { x=32, y=160, w=32, h=32, duration=3 },
        { x=64, y=160, w=32, h=32, duration=3 },
        { x=96, y=160, w=32, h=32, duration=3 }
    },
    attack_north = {
        { x=0,  y=192, w=32, h=32, duration=3 },
        { x=32, y=192, w=32, h=32, duration=3 },
        { x=64, y=192, w=32, h=32, duration=3 },
        { x=96, y=192, w=32, h=32, duration=3 }
    },
    attack_west = {
        { x=0,  y=224, w=32, h=32, duration=3 },
        { x=32, y=224, w=32, h=32, duration=3 },
        { x=64, y=224, w=32, h=32, duration=3 },
        { x=96, y=224, w=32, h=32, duration=3 }
    }
}
