#include "ball.h"
#include "entity.h"
#include "gamemath.h"
#include "intersection.h"
#include "maths.h"

static const f16 BALL_MAX_YSPEED = FIX16(32.0);

Ball Ball_init(V2f16 const pos) {
    return (Ball) {
        .position = pos,
        .velocity = { .x = f16s_0, .y = f16s_0},
    };
}

void Ball_apply_gravity(Ball *const ball) {
    Entity_apply_gravity(&ball->velocity, FIX16(0.5), BALL_MAX_YSPEED);
}

bool Ball_boundaries(Ball *const ball) {
    bool touched_floor = false;

    Entity_clamp_pos(
        &ball->position,
        &screen_clamp_min,
        &screen_clamp_max
    );

    if (ball->position.y <= screen_clamp_min.y) {
        ball->velocity.y = f16s_0;
    }
    else if (ball->position.y >= screen_clamp_max.y) {
        // TODO: go to middle x
        ball->position.y = screen_clamp_min.y;
        ball->velocity.y = f16s_0;
        touched_floor = true;
    }
    if (ball->position.x <= screen_clamp_min.x || ball->position.x >= screen_clamp_max.x) {
        ball->velocity.x = -ball->velocity.x;
    }

    return touched_floor;
}

void Ball_update(Ball *const ball, const V2f16 *const p1, const V2f16 *const p2) {
    Ball_apply_gravity(ball);
    Ball_boundaries(ball);

    V2f16 bb_size = { .x = f16s_4, .y = f16s_4 };

    if (Intersections_is_box_intersecting_seg(&ball->position, &bb_size, p1, p2)) {
        // TODO: maybe normalize could go after the dot operation...
        V2f16 rope_perp = v2_sub(p2, p1);
        rope_perp = v2_perp(&rope_perp);
        rope_perp = v2_norm(&rope_perp);

        if (rope_perp.y < f16s_0) {
            rope_perp = v2_neg(&rope_perp);
        }

        // NOTE: it was > 0.2f before, maybe we could just use 0..
        if (v2_dot(&ball->velocity, &rope_perp) > f16s_0) {
            rope_perp = v2_neg(&rope_perp);
            ball->velocity = v2_scale(&rope_perp, v2_len(&ball->velocity));
            kprintf("ball reflected!");
        }
    }

    Entity_translate(&ball->position, &ball->velocity);

    // kprintf("final ball pos: %hd.%hd %hd.%hd vel: %hd.%hd %hd.%hd",
    //     fix16ToInt(fix16Int(ball->position.x)), fix16ToInt(fix16Mul(fix16Frac(ball->position.x), FIX16(100.0))),
    //     fix16ToInt(fix16Int(ball->position.y)), fix16ToInt(fix16Mul(fix16Frac(ball->position.y), FIX16(100.0))),
    //     fix16ToInt(fix16Int(ball->velocity.x)), fix16ToInt(fix16Mul(fix16Frac(ball->velocity.x), FIX16(100.0))),
    //     fix16ToInt(fix16Int(ball->velocity.y)), fix16ToInt(fix16Mul(fix16Frac(ball->velocity.y), FIX16(100.0)))
    // );
}

void Ball_draw(const Ball *const ball) {
    const Box ball_bb = Entity_bounding_box(&ball->position, (V2u16){ .x = 8, .y = 8 });
    // kprintf("ball draw pos %hd %hd", ball_bb.x, ball_bb.y);
    // Entity_draw(
    //     &ball_bb,
    //     RGB24_TO_VDPCOLOR(0xFFFF00) // yellow
    // );
}
