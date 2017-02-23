#include <algorithm>
#include <alsa/asoundlib.h>
#include <fstream>
#include <iostream>
#include <libnotify/notify.h>
#include <memory>
#include <stdlib.h>
#include <thread>
#include <X11/Xlib.h>

using namespace std;
using namespace std::literals;

const char* ALSA_MIXER_NAME = "default";
const char* ALSA_MIXER_CONTROLLER = "PCM";
const int MAX_OUTPUT_LENGTH = 255;

const int SECONDS_PER_MINUTE = 60;
const int SECONDS_PER_HOUR = 60 * SECONDS_PER_MINUTE;
const int SECONDS_PER_DAY = 24 * SECONDS_PER_HOUR;

void AppendWithSpace(int number, string suffix, string *output) {
  if (output->length() > 0) *output += " ";
  *output += to_string(number) + suffix;
}

string FormatDuration(int seconds) {
  int days = seconds / SECONDS_PER_DAY;
  seconds %= SECONDS_PER_DAY;
  int hours = seconds / SECONDS_PER_HOUR;
  seconds %= SECONDS_PER_HOUR;
  int minutes = seconds / SECONDS_PER_MINUTE;
  seconds %= SECONDS_PER_MINUTE;

  string out;
  if (days > 0)                         AppendWithSpace(days,    "d", &out);
  if (hours > 0)                        AppendWithSpace(hours,   "h", &out);
  if (minutes > 0)                      AppendWithSpace(minutes, "m", &out);
  if (seconds > 0 || out.length() == 0) AppendWithSpace(seconds, "s", &out);
  return out;
}

int GetVolume() {
  // Create the identifier for searching for the device.
  snd_mixer_t *handle;
  snd_mixer_open(&handle, 0);
  snd_mixer_attach(handle, ALSA_MIXER_NAME);
  snd_mixer_selem_register(handle, nullptr, nullptr);
  snd_mixer_load(handle);

  snd_mixer_selem_id_t *s_elem;
  snd_mixer_selem_id_malloc(&s_elem);
  snd_mixer_selem_id_set_name(s_elem, ALSA_MIXER_CONTROLLER);

  // Find the device element.
  snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, s_elem);

  if (elem == nullptr) {
    snd_mixer_selem_id_free(s_elem);
    snd_mixer_close(handle);

    cerr << "Fatal: Could not open sound device." << endl;
    exit(1);
  }

  // Read the volume parameters.
  long int volume, min_volume, max_volume;
  snd_mixer_handle_events(handle);
  snd_mixer_selem_get_playback_volume_range(elem, &min_volume, &max_volume);
  snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_UNKNOWN, &volume);
  int percent = 100 * volume / (max_volume - min_volume);

  snd_mixer_selem_id_free(s_elem);
  snd_mixer_close(handle);

  return percent;
}

tm *GetTime() {
  time_t t = time(nullptr);
  return localtime(&t);
}

const char *WEEKDAYS[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

string FormatDate(tm *date) {
  // Date is of format "YYYY-MM-DD Day HH:MM".
  char buffer[32];
  snprintf(buffer, 32, "%04d-%02d-%02d %s %02d:%02d",
           1900 + date->tm_year, 1 + date->tm_mon, date->tm_mday,
           WEEKDAYS[date->tm_wday], date->tm_hour, date->tm_min);
  return string(buffer);
}

void SetStatus(string status) {
  Display *display = XOpenDisplay(nullptr);
  if (display) {
    XStoreName(display, DefaultRootWindow(display), status.c_str());
    XSync(display, 0);
    XCloseDisplay(display);
  } else {
    cerr << "Fatal: Could not open display." << endl;
    exit(1);
  }
}

void NotifySend(string title, string message) {
	NotifyNotification* notification =
      notify_notification_new(title.c_str(), message.c_str(), nullptr);
  GError *err = nullptr;
	if (!notify_notification_show(notification, &err)) {
    cerr << "Warning: Failed to send notification: " << err->message << endl;
  }
	g_object_unref(G_OBJECT(notification));
}

int main(int argc, char *args[]) {
  if (!notify_init("Status Bar")) {
    cerr << "Fatal: Could not initialise libnotify." << endl;
    exit(1);
  }

  // Output buffer.
  int buffer_size = MAX_OUTPUT_LENGTH + 1;
  unique_ptr<char[]> output_buffer(new char[buffer_size]);

  int iteration = 0;  // Incremented once every iteration, mod 10.

  while (true) {
    // Calculate the current state variables.
    int volume = GetVolume();
    int round_size = 5;
    int rounded_volume = (volume + round_size/2);
    rounded_volume -= rounded_volume % round_size;

    string date = FormatDate(GetTime());

    snprintf(output_buffer.get(), buffer_size, "[Headphones - %d%%] [%s]", 
             rounded_volume, date.c_str());

    // Transfer it to the status bar.
    SetStatus(output_buffer.get());

    this_thread::sleep_for(100ms);
    iteration = (iteration + 1) % 10;
  }

  return 0;
}
