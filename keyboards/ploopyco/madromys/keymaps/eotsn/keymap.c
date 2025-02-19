/* Copyright 2025 Eric Ottosson <noreply@eotsn.com>
 * Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum {
    BTN5_DRAG_SCROLL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN4, DPI_CONFIG, TG(1), KC_BTN1, KC_BTN5,              KC_BTN2 ),
    [1] = LAYOUT( KC_BTN4, DPI_CONFIG, TO(0), KC_BTN1, TD(BTN5_DRAG_SCROLL), KC_BTN2 ),
};

// Fake `keyrecord_t` for hooking `process_record_kb()` with custom keycodes.
static keyrecord_t fake_record;

void btn5_drag_scroll_finished(tap_dance_state_t *state, void *user_data) {
    if (state->pressed) {
        if (state->count == 1) {
            fake_record.event.key.col = 0;
            fake_record.event.key.row = 0;
            fake_record.event.pressed = true;
            fake_record.event.time = timer_read();
            process_record_kb(DRAG_SCROLL, &fake_record);
        }
    }
}

void btn5_drag_scroll_reset(tap_dance_state_t *state, void *user_data) {
    fake_record.event.key.col = 0;
    fake_record.event.key.row = 0;
    fake_record.event.pressed = false;
    fake_record.event.time = timer_read();
    process_record_kb(DRAG_SCROLL, &fake_record);
}

tap_dance_action_t tap_dance_actions[] = {
    [BTN5_DRAG_SCROLL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, btn5_drag_scroll_finished, btn5_drag_scroll_reset),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(BTN5_DRAG_SCROLL):
            action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_code16(KC_BTN5);
            }
    }
    return true;
}
