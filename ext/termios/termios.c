#include <termios.h>

#include "ruby.h"
#include "ruby/io.h"

static VALUE cTermios;

static void termios_free(void * data) {
	free(data);
}

static size_t termios_size(const void * data) {
	return sizeof(struct termios);
}

static const rb_data_type_t termios_type = {
	.wrap_struct_name = "termios",
	.function = {
		.dmark = NULL,
		.dfree = termios_free,
		.dsize = termios_size,
	},
	.data = NULL,
	.flags = RUBY_TYPED_FREE_IMMEDIATELY,
};


static VALUE termios_alloc(VALUE self) {
	struct termios * data = malloc(sizeof(struct termios));

	return TypedData_Wrap_Struct(self, &termios_type, data);
}

static struct termios * get_data(VALUE termios) {
  struct termios * data;

	TypedData_Get_Struct(termios, struct termios, &termios_type, data);

  return data;
}

static void check_errno(int value, const char * msg) {
  if (value != 0) {
    rb_sys_fail(msg);
  }
}

static VALUE termios_getattr(VALUE self, VALUE io) {
  VALUE termios = termios_alloc(cTermios);

  check_errno(tcgetattr(rb_io_descriptor(io), get_data(termios)), "tcgetattr failed");

  return termios;
}

static VALUE termios_setattr(VALUE self, VALUE io, VALUE optional_actions, VALUE termios) {
  check_errno(tcsetattr(rb_io_descriptor(io), FIX2INT(optional_actions), get_data(termios)), "tcsetattr failed");

  return Qnil;
}

static VALUE termios_drain(VALUE self, VALUE io) {
  check_errno(tcdrain(rb_io_descriptor(io)), "tcdrain failed");

  return Qnil;
}

static VALUE termios_flow(VALUE self, VALUE io, VALUE action) {
  check_errno(tcflow(rb_io_descriptor(io), FIX2INT(action)), "tcflow failed");

  return Qnil;
}

static VALUE termios_flush(VALUE self, VALUE io, VALUE queue_selector) {
  check_errno(tcflush(rb_io_descriptor(io), FIX2INT(queue_selector)), "tcflush failed");
  
  return Qnil;
}

static VALUE termios_sendbreak(VALUE self, VALUE io, VALUE duration) {
  check_errno(tcsendbreak(rb_io_descriptor(io), FIX2INT(duration)), "sendbreak failed");

  return Qnil;
}

static VALUE termios_initialize(VALUE self) {
  memset(get_data(self), 0, sizeof(struct termios));

	return self;
}

static VALUE termios_get_ispeed(VALUE self) {
  return LONG2NUM(cfgetispeed(get_data(self)));
}

static VALUE termios_set_ispeed(VALUE self, VALUE speed) {
  return LONG2NUM(cfsetispeed(get_data(self), NUM2LONG(speed)));
}

static VALUE termios_get_ospeed(VALUE self) {
  return LONG2NUM(cfgetospeed(get_data(self)));
}

static VALUE termios_set_ospeed(VALUE self, VALUE speed) {
  return ULONG2NUM(cfsetospeed(get_data(self), NUM2LONG(speed)));
}

static VALUE termios_get_iflag(VALUE self) {
  return ULONG2NUM(get_data(self)->c_iflag);
}

static VALUE termios_set_iflag(VALUE self, VALUE flag) {
  get_data(self)->c_iflag = NUM2ULONG(flag);

  return termios_get_iflag(self);
}

static VALUE termios_get_oflag(VALUE self) {
  return ULONG2NUM(get_data(self)->c_oflag);
}

static VALUE termios_set_oflag(VALUE self, VALUE flag) {
  get_data(self)->c_oflag = NUM2ULONG(flag);

  return termios_get_oflag(self);
}

static VALUE termios_get_cflag(VALUE self) {
  return ULONG2NUM(get_data(self)->c_cflag);
}

static VALUE termios_set_cflag(VALUE self, VALUE flag) {
  get_data(self)->c_cflag = NUM2ULONG(flag);

  return termios_get_cflag(self);
}

static VALUE termios_get_lflag(VALUE self) {
  return ULONG2NUM(get_data(self)->c_lflag);
}

static VALUE termios_set_lflag(VALUE self, VALUE flag) {
  get_data(self)->c_lflag = NUM2ULONG(flag);

  return termios_get_lflag(self);
}

// TODO: Add the following
// cc_t      c_cc[NCCS]  Control characters. 

#define CONSTANT(x) rb_define_const(cTermios, #x, INT2FIX(x));

void Init_termios(void) {
  VALUE terminal = rb_define_module("TerminalIO");

  cTermios = rb_define_class_under(terminal, "Termios", rb_cObject);

  rb_define_alloc_func(cTermios, termios_alloc);

  // Input Modes
  CONSTANT(BRKINT);
  CONSTANT(ICRNL);
  CONSTANT(IGNBRK);
  CONSTANT(IGNCR);
  CONSTANT(IGNPAR);
  CONSTANT(INLCR);
  CONSTANT(INPCK);
  CONSTANT(ISTRIP);
  CONSTANT(IXANY);
  CONSTANT(IXOFF);
  CONSTANT(IXON);
  CONSTANT(PARMRK);

  // Output Modes
  CONSTANT(OPOST);
  CONSTANT(ONLCR);
  CONSTANT(OCRNL);
  CONSTANT(ONOCR);
  CONSTANT(ONLRET);
  CONSTANT(OFDEL);
  CONSTANT(OFILL);
  CONSTANT(NLDLY);
  CONSTANT(NL0);
  CONSTANT(NL1);
  CONSTANT(CRDLY);
  CONSTANT(CR0);
  CONSTANT(CR1);
  CONSTANT(CR2);
  CONSTANT(CR3);
  CONSTANT(TABDLY);
  CONSTANT(TAB0);
  CONSTANT(TAB1);
  CONSTANT(TAB2);
  CONSTANT(TAB3);
  CONSTANT(BSDLY);
  CONSTANT(BS0);
  CONSTANT(BS1);
  CONSTANT(VTDLY);
  CONSTANT(VT0);
  CONSTANT(VT1);
  CONSTANT(FFDLY);
  CONSTANT(FF0);
  CONSTANT(FF1);

  // Baud Rates
  //
  // Note: These are completely useless
  //       They are just here because someone will ask why they are not here otherwise, just use an integer value instead
  CONSTANT(B0);
  CONSTANT(B50);
  CONSTANT(B75);
  CONSTANT(B110);
  CONSTANT(B134);
  CONSTANT(B150);
  CONSTANT(B200);
  CONSTANT(B300);
  CONSTANT(B600);
  CONSTANT(B1200);
  CONSTANT(B1800);
  CONSTANT(B2400);
  CONSTANT(B4800);
  CONSTANT(B9600);
  CONSTANT(B19200);
  CONSTANT(B38400);

  // Control Modes
  CONSTANT(CSIZE);
  CONSTANT(CS5);
  CONSTANT(CS6);
  CONSTANT(CS7);
  CONSTANT(CS8);
  CONSTANT(CSTOPB);
  CONSTANT(CREAD);
  CONSTANT(PARENB);
  CONSTANT(PARODD);
  CONSTANT(HUPCL);
  CONSTANT(CLOCAL);

  // Local Modes
  CONSTANT(ECHO);
  CONSTANT(ECHOE);
  CONSTANT(ECHOK);
  CONSTANT(ECHONL);
  CONSTANT(ICANON);
  CONSTANT(IEXTEN);
  CONSTANT(ISIG);
  CONSTANT(NOFLSH);
  CONSTANT(TOSTOP);

  // Attribute Selection
  CONSTANT(TCSANOW);
  CONSTANT(TCSADRAIN);
  CONSTANT(TCSAFLUSH);

  // Line Control
  CONSTANT(TCIFLUSH);
  CONSTANT(TCIOFLUSH);
  CONSTANT(TCOFLUSH);
  CONSTANT(TCIOFF);
  CONSTANT(TCION);
  CONSTANT(TCOOFF);
  CONSTANT(TCOON);

  rb_define_singleton_method(cTermios, "getattr", termios_getattr, 1);
  rb_define_singleton_method(cTermios, "setattr", termios_setattr, 3);
  rb_define_singleton_method(cTermios, "drain", termios_drain, 1);
  rb_define_singleton_method(cTermios, "flow", termios_flow, 2);
  rb_define_singleton_method(cTermios, "flush", termios_flush, 2);
  rb_define_singleton_method(cTermios, "sendbreak", termios_sendbreak, 2);

	rb_define_method(cTermios, "initialize", termios_initialize, 0);

	rb_define_method(cTermios, "ispeed", termios_get_ispeed, 0);
	rb_define_method(cTermios, "ospeed", termios_get_ospeed, 0);
	rb_define_method(cTermios, "iflag", termios_get_iflag, 0);
	rb_define_method(cTermios, "oflag", termios_get_oflag, 0);
	rb_define_method(cTermios, "cflag", termios_get_cflag, 0);
	rb_define_method(cTermios, "lflag", termios_get_lflag, 0);

	rb_define_method(cTermios, "ispeed=", termios_set_ispeed, 1);
	rb_define_method(cTermios, "ospeed=", termios_set_ospeed, 1);
	rb_define_method(cTermios, "iflag=", termios_set_iflag, 1);
	rb_define_method(cTermios, "oflag=", termios_set_oflag, 1);
	rb_define_method(cTermios, "cflag=", termios_set_cflag, 1);
	rb_define_method(cTermios, "lflag=", termios_set_lflag, 1);
}
