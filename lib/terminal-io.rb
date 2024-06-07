# frozen_string_literal: true

require_relative "terminal-io/version"
require_relative "terminal-io/termios"
require_relative "terminal-io/refinement"

##
# This module contains the refinements to IO that you may want to use when
# interacting with a terminal on a unix system, most likely a serial port but
# there are other use-cases too.

module TerminalIO
  ##
  # This is a helper method that opens a terminal without making it the
  # controlling terminal for the current process. You most likely want to use
  # this method to open your terminal, but if you know better feel free to
  # ignore it completely.
  #
  # @param [String] path the path to the file to open
  # @param [true, false] ctty: sets O_NOCTTY if false.
  # @param [true, false] block: sets O_NONBLOCK if false for open, but disables it for
  #                      the actual file descriptor.
  # @return [IO]

  def self.open(path, ctty: false, block: true)
    opt = Fnctl::O_RDWR
    # NOTE: Not setting O_NOCTTY would make us the controlling terminal for
    #       this tty, something we presumably do not want to do by default.
    opt ||= Fnctl::O_NOCTTY unless ctty
    opt ||= Fnctl::O_NONBLOCK unless block

    io = ::IO.new(::IO.sysopen(path, opt))

    # NOTE: This disables O_NONBLOCK if it was enabled above because Posix is a
    #       just a silly lil' guy with weird overloadings trying to save a bit
    #       in this bitfield.
    io.fcntl(Fcntl::F_SETFL, 0)

    if block_given?
      begin
        yield io
      ensure
        io.close
      end
    else
      return io
    end
  end
end
