# frozen_string_literal: true

using TerminalIO

RSpec.describe TerminalIO do
  let(:tty) { $stdin }
  let(:file) { File.open("/dev/null") }

  it "has a version number" do
    expect(TerminalIO::VERSION).not_to be nil
  end

  it "should fail on files" do
    expect { file.getattr }.to raise_error(Errno::ENODEV)
  end

  it "should succeed on ttys" do
    expect(tty.getattr).to be_instance_of(TerminalIO::Termios)
  end

  it "should not crash when doing a bunch of bullshit" do
    expect do
      tty.flow_suspend
      tty.flow_resume
      tty.flush_queue
    end.not_to raise_error
  end
end
