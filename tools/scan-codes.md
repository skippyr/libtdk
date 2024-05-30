<p align="center">
  <img alt="" src="assets/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;libtdk&ensp;≥v≥v</h1>
<p align="center">Terminal Development Kit - Documentation - Scan Codes</p>

## ❡ Mouse Events

### Codes

Mouse events on POSIX systems must be enabled, modified and disabled manually by using these codes:

<table align="center">
  <thead>
    <tr>
      <td>ANSI</td>
      <td>Description</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>x1b[?1000h</code> and <code>\x1b[?1000l</code></td>
      <td>Enables/disables the reading of only button events.</td>
    </tr>
    <tr>
      <td><code>x1b[?1002h</code> and <code>\x1b[?1002l</code></td>
      <td>Enables/disables the reading of button and dragging events.</td>
    </tr>
    <tr>
      <td><code>\x1b[?1003h</code> and <code>\x1b[?1003l</code></td>
      <td>Enables/disables the reading of all events: button and dragging, even without any button being hold.</td>
    </tr>
    <tr>
      <td><code>\x1b[?1006h</code> and <code>\x1b[?1006l</code></td>
      <td>Increase the range of events detection and modify the syntax of ANSI codes received.</td>
    </tr>
  </tbody>
</table>

### Mode

On Windows, mouse events are enabled by setting `ENABLE_MOUSE_INPUT` and removing `ENABLE_QUICK_EDIT_MODE` flags from the standard `HANDLE` obtained from the `STD_INPUT_HANDLE` value. They are disabled by doing the opposite.
