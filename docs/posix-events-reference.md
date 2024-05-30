<p align="center">
  <img alt="" src="../assets/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;libtmk&ensp;≥v≥v</h1>
<p align="center">Terminal Manipulation Kit - Documentation - POSIX Events Reference<br /><strong>Internal Use</strong></p>

## ❡ About

This documentation explains how some POSIX event types are handled at a low-level perspective and contains reference codes used to parse them internally.

## ❡ Window Resize Events

Window resizes are not an event, but actually the `SIGWINCH` signal send to the terminal signal stack asynchnously. The library parses that signal at the same time as other events due to the use of the `ppoll` system call, which allows it to wait for input and handle non-blocked signals at the same time.

## ❡ Focus Events

Focus events, in order to be parsed, must be manually enabled and disabled by using the codes `\x1b[?1004h` and `\x1b[?1004l`, respectively.

When enabled, focus events are reported by using the following codes:

<table align="center">
<thead>
  <tr>
    <th>ANSI</th>
    <th>Description</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>27 91 73</td>
    <td>The terminal window has gained focus.</td>
  </tr>
  <tr>
    <td>27 91 79</td>
    <td>The terminal window has lost focus.</td>
  </tr>
</tbody>
</table>
<p align="center"><strong>Caption:</strong> the ANSI codes of focus events.</p>

## ❡ Mouse Events

### Enable/Disable Codes

Mouse events, in order to be parsed, must be manually enabled and disabled by using the following codes:

<table align="center">
<thead>
  <tr>
    <th>ANSI</th>
    <th>Description</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td><code>\x1b[?1000h</code> and <code>\x1b[?1000l</code></td>
    <td>Enables/disables the reading of only click events.</td>
  </tr>
  <tr>
    <td><code>\x1b[?1002h</code> and <code>\x1b[?1002l</code></td>
    <td>Enables/disables the reading of click and movement events.</td>
  </tr>
  <tr>
    <td><code>\x1b[?1003h</code> and <code>\x1b[?1003l</code></td>
    <td>Enables/disables the reading of all mouse events: button and movement events, even without any button being hold.</td>
  </tr>
  <tr>
    <td><code>\x1b[?1006h</code> and <code>\x1b[?1006l</code></td>
    <td>Increases/deacreses the range of reading and modifies the syntax of ANSI codes received.</td>
  </tr>
</tbody>
</table>
<p align="center"><strong>Caption:</strong> the ANSI codes used to enable/disable mouse events.</p>

### Syntax

All mouse events reported by using the `\x1b[?1006h` code, which is what this library uses, use the following syntax:

```
27 91 60 <EVENT INFO> 59 <COORDINATE COLUMN> 59 <COORDINATE ROW> <RELEASE STATE>
```

### Event Info

The ```<EVENT INFO>``` field contains information about the event type, button and modifier keys used.

#### Click Events

Click events are reported as the following codes:

<table align="center">
  <thead>
    <tr>
      <th>Button</th>
      <th>No Modifier</th>
      <th>Ctrl</th>
      <th>Alt</th>
      <th>Shift</th>
      <th>Ctrl + Alt</th>
      <th>Ctrl + Shift</th>
      <th>Alt + Shift</th>
      <th>Ctrl + Alt + Shift</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Left</td>
      <td>48</td>
      <td>49 54</td>
      <td>56</td>
      <td>N/A</td>
      <td>50 52</td>
      <td>N/A</td>
      <td>49 50</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td>Middle</td>
      <td>49</td>
      <td>49 55</td>
      <td>57</td>
      <td>N/A</td>
      <td>50 53</td>
      <td>50 49</td>
      <td>49 51</td>
      <td>50 57</td>
    </tr>
    <tr>
      <td>Right</td>
      <td>50</td>
      <td>49 56</td>
      <td>49 48</td>
      <td>N/A</td>
      <td>50 54</td>
      <td>50 50</td>
      <td>49 52</td>
      <td>51 48</td>
    </tr>
  </tbody>
</table>
<p align="center"><strong>Caption:</strong> The ANSI codes of click events.</p>

### Dragging Events

Dragging events are reported as the following codes:

<table align="center">
  <thead>
    <tr>
      <th>Button</th>
      <th>No Modifier</th>
      <th>Ctrl</th>
      <th>Alt</th>
      <th>Shift</th>
      <th>Ctrl + Alt</th>
      <th>Ctrl + Shift</th>
      <th>Alt + Shift</th>
      <th>Ctrl + Alt + Shift</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>None</td>
      <td>51 53</td>
      <td>53 49</td>
      <td>52 51</td>
      <td>51 57</td>
      <td>53 57</td>
      <td>53 53</td>
      <td>52 55</td>
      <td>54 51</td>
    </tr>
    <tr>
      <td>Left</td>
      <td>51 50</td>
      <td>52 56</td>
      <td>52 48</td>
      <td>N/A</td>
      <td>53 54</td>
      <td>53 50</td>
      <td>52 52</td>
      <td>54 48</td>
    </tr>
    <tr>
      <td>Middle</td>
      <td>51 51</td>
      <td>52 57</td>
      <td>52 49</td>
      <td>51 55</td>
      <td>53 55</td>
      <td>53 51</td>
      <td>52 53</td>
      <td>52 49</td>
    </tr>
    <tr>
      <td>Right</td>
      <td>51 52</td>
      <td>53 48</td>
      <td>52 50</td>
      <td>N/A</td>
      <td>53 56</td>
      <td>53 52</td>
      <td>52 54</td>
      <td>54 50</td>
    </tr>
  </tbody>
</table>
<p align="center"><strong>Caption:</strong> The ANSI codes of dragging events.</p>

### Coordinates

The `<COORDINATE COLUMN>` and `<COORDINATE ROW>` fields contain the components of the mouse coordinate. Coordinates are indicated by using numbers from range 48 to 57. Increase/decreasing operations always happen to the number at the far right side. The first value starts as 49 and when it reaches the limit:

- If it is the first number or its the number at its left side reaches maximum, it resets to 49 and add a new value to its right, starting as 48.
- Otherwise, it increases its left side number by one and gets reset to 48.

### Release States

The `<RELEASE STATE>` field contains the current release state of a possible button. It can be one of the following codes:

<table align="center">
  <thead>
    <th>ANSI</th>
    <th>Description</th>
  </thead>
  <tbody>
    <tr>
      <td>77</td>
      <td>No button was released: it means a press state when a click event has happened.</td>
    </tr>
    <tr>
      <td>109</td>
      <td>A button was released.</td>
    </tr>
  </tbody>
</table>
<p align="center"><strong>Caption:</strong> The ANSI codes of release states.</p>

## ❡ Key Events

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
