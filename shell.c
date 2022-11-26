#include "shell.h"
#include "string.h"
#include "fat.h"
#include "lib/print.h"
#include "putc.h"

#define SIZEOF_ARRAY(arr) (sizeof arr / sizeof *arr)

const char *prompt = "shell> ";

struct command
{
	const char *name;
	void (*function)(const char *param);
};

static struct command commands[] = {
	{
		.name = "ls",
		.function = print_dir
	},
	{
		.name = "cat",
		.function = print_file
	},
	{
		.name = "cd",
		.function = change_dir
	},
};

static char buffer[20];
static unsigned buffer_idx;

static void reset_buffer(void)
{
	for (unsigned i = 0; i < sizeof buffer; ++i)
		buffer[i] = 0;
	buffer_idx = 0;
}

void shell_reset(void)
{
	reset_buffer();
	printk("%s", prompt);
}

void shell_feed_char(char c)
{
	if (buffer_idx >= sizeof buffer)
		shell_reset();
	putc(c);
	buffer[buffer_idx++] = c;
	if ('\n' != c)
		return;
	buffer[buffer_idx - 2] = 0;
	for (unsigned i = 0; i < SIZEOF_ARRAY(commands); ++i)
		if (0 == strncmp(commands[i].name, buffer, strlen(commands[i].name)))
		{
			commands[i].function(buffer + strlen(commands[i].name) + 1);
			break;
		}
	shell_reset();
}

static char *get_last_token(void)
{
	for (unsigned i = buffer_idx - 1; i; --i)
		if (buffer[i] == ' ')
			return &buffer[i + 1];
	return buffer;
}

void shell_autocomplete(void)
{
	char *last_token = get_last_token();
	struct fat_directory *entry = find_starts_with(last_token);

	if (!entry)
		return;

	shell_move_cursor(last_token - buffer - buffer_idx);
	for (unsigned i = 0; i < sizeof entry->filename; ++i)
		shell_feed_char(entry->filename[i]);
}

void shell_move_cursor(int n)
{
	move_cursor(n);
	buffer_idx += n;

}
void shell_backspace(void)
{
	if (!buffer_idx)
		return;
	buffer_idx--;
	buffer[buffer_idx] = 0;
	move_cursor(-1);
	putc(' ');
	move_cursor(-1);
}
