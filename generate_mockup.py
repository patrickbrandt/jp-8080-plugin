#!/usr/bin/env python3
"""Generate JP-8080 Controller GUI mockup as JPEG image."""

from PIL import Image, ImageDraw, ImageFont
import math

# Constants
WIDTH = 1100
HEIGHT = 750
BG_COLOR = (26, 26, 46)
PANEL_COLOR = (45, 45, 68)
SECTION_COLOR = (37, 37, 56)
BORDER_COLOR = (58, 58, 90)
RED_ACCENT = (230, 57, 70)
CYAN_TEXT = (168, 218, 220)
WHITE_TEXT = (241, 250, 238)
GREEN_LED = (0, 255, 136)
DARK_BG = (10, 10, 20)

def draw_rounded_rect(draw, xy, radius, fill, outline=None):
    """Draw a rounded rectangle."""
    x1, y1, x2, y2 = xy
    draw.rectangle([x1 + radius, y1, x2 - radius, y2], fill=fill)
    draw.rectangle([x1, y1 + radius, x2, y2 - radius], fill=fill)
    draw.ellipse([x1, y1, x1 + 2*radius, y1 + 2*radius], fill=fill)
    draw.ellipse([x2 - 2*radius, y1, x2, y1 + 2*radius], fill=fill)
    draw.ellipse([x1, y2 - 2*radius, x1 + 2*radius, y2], fill=fill)
    draw.ellipse([x2 - 2*radius, y2 - 2*radius, x2, y2], fill=fill)
    if outline:
        # Draw outline arcs and lines
        draw.arc([x1, y1, x1 + 2*radius, y1 + 2*radius], 180, 270, fill=outline)
        draw.arc([x2 - 2*radius, y1, x2, y1 + 2*radius], 270, 360, fill=outline)
        draw.arc([x1, y2 - 2*radius, x1 + 2*radius, y2], 90, 180, fill=outline)
        draw.arc([x2 - 2*radius, y2 - 2*radius, x2, y2], 0, 90, fill=outline)
        draw.line([x1 + radius, y1, x2 - radius, y1], fill=outline)
        draw.line([x1 + radius, y2, x2 - radius, y2], fill=outline)
        draw.line([x1, y1 + radius, x1, y2 - radius], fill=outline)
        draw.line([x2, y1 + radius, x2, y2 - radius], fill=outline)

def draw_knob(draw, cx, cy, size=40, rotation=-45):
    """Draw a knob with indicator."""
    # Outer ring
    draw.ellipse([cx - size//2, cy - size//2, cx + size//2, cy + size//2],
                 fill=(58, 58, 90), outline=(74, 74, 106))
    # Inner circle
    inner_size = size - 8
    draw.ellipse([cx - inner_size//2, cy - inner_size//2, cx + inner_size//2, cy + inner_size//2],
                 fill=(45, 45, 68))
    # Indicator line
    angle = math.radians(rotation - 90)
    line_len = size // 2 - 6
    x2 = cx + int(math.cos(angle) * line_len)
    y2 = cy + int(math.sin(angle) * line_len)
    draw.line([cx, cy, x2, y2], fill=RED_ACCENT, width=3)

def draw_section(draw, x, y, w, h, title, knobs, font_title, font_label, dropdown=None):
    """Draw a section panel with title and knobs."""
    # Background
    draw_rounded_rect(draw, [x, y, x + w, y + h], 8, SECTION_COLOR, BORDER_COLOR)

    # Title
    draw.text((x + 10, y + 8), title, fill=RED_ACCENT, font=font_title)
    draw.line([x + 10, y + 25, x + w - 10, y + 25], fill=BORDER_COLOR)

    # Dropdown if present
    start_y = y + 35
    if dropdown:
        draw_rounded_rect(draw, [x + 20, start_y, x + w - 20, start_y + 25], 4, DARK_BG, (69, 123, 157))
        draw.text((x + 30, start_y + 5), dropdown, fill=WHITE_TEXT, font=font_label)
        start_y += 35

    # Knobs
    knob_y = start_y + 30
    knob_spacing = w // (len(knobs) + 1)
    for i, (label, rotation) in enumerate(knobs):
        knob_x = x + knob_spacing * (i + 1)
        draw_knob(draw, knob_x, knob_y, rotation=rotation)
        # Label
        label_w = len(label) * 5
        draw.text((knob_x - label_w//2, knob_y + 28), label, fill=CYAN_TEXT, font=font_label)

def draw_envelope_curve(draw, x, y, w, h):
    """Draw an envelope visualization."""
    draw_rounded_rect(draw, [x, y, x + w, y + h], 4, DARK_BG)
    # ADSR curve
    points = [
        (x + 5, y + h - 5),
        (x + w * 0.15, y + 8),
        (x + w * 0.4, y + h * 0.4),
        (x + w * 0.7, y + h * 0.4),
        (x + w - 5, y + h - 5)
    ]
    for i in range(len(points) - 1):
        draw.line([points[i], points[i+1]], fill=RED_ACCENT, width=2)

def main():
    # Create image
    img = Image.new('RGB', (WIDTH, HEIGHT), BG_COLOR)
    draw = ImageDraw.Draw(img)

    # Try to load fonts, fall back to default
    try:
        font_title = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 11)
        font_logo = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 24)
        font_label = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 9)
        font_patch = ImageFont.truetype("/Library/Fonts/Courier New.ttf", 14)
    except:
        font_title = ImageFont.load_default()
        font_logo = ImageFont.load_default()
        font_label = ImageFont.load_default()
        font_patch = ImageFont.load_default()

    # Main panel
    draw_rounded_rect(draw, [15, 15, WIDTH - 15, HEIGHT - 15], 12, PANEL_COLOR, BORDER_COLOR)

    # Header
    draw.text((35, 30), "JP-8080", fill=RED_ACCENT, font=font_logo)
    draw.text((145, 35), "CONTROLLER", fill=WHITE_TEXT, font=font_title)
    draw.line([35, 60, WIDTH - 35, 60], fill=RED_ACCENT, width=2)

    # MIDI Channel selector (top right)
    draw.text((WIDTH - 200, 35), "MIDI CH:", fill=CYAN_TEXT, font=font_label)
    draw_rounded_rect(draw, [WIDTH - 150, 30, WIDTH - 110, 50], 4, DARK_BG, (69, 123, 157))
    draw.text((WIDTH - 140, 35), "1", fill=WHITE_TEXT, font=font_label)

    # LED indicator
    draw.ellipse([WIDTH - 220, 35, WIDTH - 210, 45], fill=GREEN_LED)

    # Section dimensions
    section_w = 340
    section_h = 150
    gap = 15
    start_x = 35
    start_y = 80

    # Row 1: Oscillators
    # OSC 1
    draw_section(draw, start_x, start_y, section_w, section_h, "OSCILLATOR 1",
                [("CTRL 1", -60), ("CTRL 2", 30)], font_title, font_label, "SAW")

    # OSC 2
    draw_section(draw, start_x + section_w + gap, start_y, section_w, section_h, "OSCILLATOR 2",
                [("RANGE", -90), ("FINE", 0), ("CTRL 1", -45), ("CTRL 2", 60)], font_title, font_label, "SAW")

    # OSC Common
    draw_section(draw, start_x + 2*(section_w + gap), start_y, section_w, section_h, "OSC COMMON",
                [("BALANCE", 0), ("X-MOD", -80), ("LFO DPT", -40)], font_title, font_label)

    # Row 2: Filter and Envelopes
    row2_y = start_y + section_h + gap

    # Filter
    draw_section(draw, start_x, row2_y, section_w, section_h, "FILTER",
                [("CUTOFF", 45), ("RESO", -30), ("KEY FOL", 0), ("LFO DPT", -60)], font_title, font_label, "LPF 24dB")

    # Filter Envelope
    x2 = start_x + section_w + gap
    draw_rounded_rect(draw, [x2, row2_y, x2 + section_w, row2_y + section_h], 8, SECTION_COLOR, BORDER_COLOR)
    draw.text((x2 + 10, row2_y + 8), "FILTER ENVELOPE", fill=RED_ACCENT, font=font_title)
    draw.line([x2 + 10, row2_y + 25, x2 + section_w - 10, row2_y + 25], fill=BORDER_COLOR)
    knobs = [("DEPTH", 20), ("A", -100), ("D", -40), ("S", 30), ("R", -20)]
    knob_y = row2_y + 55
    knob_spacing = section_w // 6
    for i, (label, rotation) in enumerate(knobs):
        knob_x = x2 + knob_spacing * (i + 1)
        draw_knob(draw, knob_x, knob_y, size=35, rotation=rotation)
        draw.text((knob_x - 5, knob_y + 22), label, fill=CYAN_TEXT, font=font_label)
    draw_envelope_curve(draw, x2 + 10, row2_y + 100, section_w - 20, 40)

    # Amp Envelope
    x3 = start_x + 2*(section_w + gap)
    draw_rounded_rect(draw, [x3, row2_y, x3 + section_w, row2_y + section_h], 8, SECTION_COLOR, BORDER_COLOR)
    draw.text((x3 + 10, row2_y + 8), "AMP ENVELOPE", fill=RED_ACCENT, font=font_title)
    draw.line([x3 + 10, row2_y + 25, x3 + section_w - 10, row2_y + 25], fill=BORDER_COLOR)
    knobs = [("LEVEL", 60), ("A", -90), ("D", -30), ("S", 45), ("R", -50)]
    for i, (label, rotation) in enumerate(knobs):
        knob_x = x3 + knob_spacing * (i + 1)
        draw_knob(draw, knob_x, knob_y, size=35, rotation=rotation)
        draw.text((knob_x - 5, knob_y + 22), label, fill=CYAN_TEXT, font=font_label)
    draw_envelope_curve(draw, x3 + 10, row2_y + 100, section_w - 20, 40)

    # Row 3: LFOs and Effects
    row3_y = row2_y + section_h + gap
    small_section_w = 220

    # LFO 1
    draw_section(draw, start_x, row3_y, small_section_w, 120, "LFO 1",
                [("RATE", -20), ("FADE", -90)], font_title, font_label, "TRIANGLE")

    # LFO 2
    draw_section(draw, start_x + small_section_w + gap, row3_y, small_section_w, 120, "LFO 2",
                [("RATE", 10), ("DEPTH", -60)], font_title, font_label, "PITCH")

    # Pitch Envelope
    draw_section(draw, start_x + 2*(small_section_w + gap), row3_y, small_section_w, 120, "PITCH ENV",
                [("DEPTH", -80), ("A", -100), ("D", -40)], font_title, font_label)

    # Effects (wider)
    effects_x = start_x + 3*(small_section_w + gap)
    effects_w = WIDTH - 35 - effects_x
    draw_rounded_rect(draw, [effects_x, row3_y, effects_x + effects_w, row3_y + 120], 8, SECTION_COLOR, BORDER_COLOR)
    draw.text((effects_x + 10, row3_y + 8), "EFFECTS", fill=RED_ACCENT, font=font_title)
    draw.line([effects_x + 10, row3_y + 25, effects_x + effects_w - 10, row3_y + 25], fill=BORDER_COLOR)
    fx_knobs = [("BASS", -60), ("TREBLE", 30), ("FX LVL", 45), ("DLY TM", -30), ("DLY FB", -50), ("DLY LV", 20)]
    fx_spacing = effects_w // 7
    for i, (label, rotation) in enumerate(fx_knobs):
        knob_x = effects_x + fx_spacing * (i + 1)
        draw_knob(draw, knob_x, row3_y + 70, size=35, rotation=rotation)
        draw.text((knob_x - 15, row3_y + 98), label, fill=CYAN_TEXT, font=font_label)
    # Divider between tone control and delay
    draw.line([effects_x + fx_spacing * 3 + 15, row3_y + 40, effects_x + fx_spacing * 3 + 15, row3_y + 110], fill=BORDER_COLOR)

    # Patch Browser (bottom)
    patch_y = row3_y + 130
    draw_rounded_rect(draw, [start_x, patch_y, WIDTH - 35, patch_y + 50], 8, (21, 21, 37), BORDER_COLOR)

    draw.text((start_x + 15, patch_y + 18), "PATCH:", fill=RED_ACCENT, font=font_title)

    # Bank selector
    draw_rounded_rect(draw, [start_x + 70, patch_y + 12, start_x + 140, patch_y + 38], 4, DARK_BG, (69, 123, 157))
    draw.text((start_x + 80, patch_y + 18), "User", fill=WHITE_TEXT, font=font_label)

    # Group selector
    draw_rounded_rect(draw, [start_x + 150, patch_y + 12, start_x + 190, patch_y + 38], 4, DARK_BG, (69, 123, 157))
    draw.text((start_x + 165, patch_y + 18), "A", fill=WHITE_TEXT, font=font_label)

    # Patch display
    draw_rounded_rect(draw, [start_x + 200, patch_y + 10, start_x + 420, patch_y + 40], 4, DARK_BG, (69, 123, 157))
    draw.text((start_x + 215, patch_y + 16), "A-11 Super Saw Lead", fill=GREEN_LED, font=font_patch)

    # Nav buttons
    for i, symbol in enumerate(["<", ">"]):
        bx = start_x + 440 + i * 40
        draw_rounded_rect(draw, [bx, patch_y + 12, bx + 30, patch_y + 38], 4, (58, 58, 90), (74, 74, 106))
        draw.text((bx + 10, patch_y + 16), symbol, fill=WHITE_TEXT, font=font_title)

    # Action buttons
    draw_rounded_rect(draw, [WIDTH - 180, patch_y + 12, WIDTH - 120, patch_y + 38], 4, (58, 58, 90), (74, 74, 106))
    draw.text((WIDTH - 165, patch_y + 18), "LOAD", fill=WHITE_TEXT, font=font_label)

    draw_rounded_rect(draw, [WIDTH - 110, patch_y + 12, WIDTH - 50, patch_y + 38], 4, RED_ACCENT)
    draw.text((WIDTH - 95, patch_y + 18), "SEND", fill=WHITE_TEXT, font=font_label)

    # Save image
    output_path = "/Users/patrickbrandt/Projects/jp-8080-plugin/gui-mockup.jpg"
    img.save(output_path, "JPEG", quality=95)
    print(f"Saved: {output_path}")

if __name__ == "__main__":
    main()
