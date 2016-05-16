
/*! \page UI UI Module Reference

<h2>Windows</h2>
<ul>
<li>uicore::WindowManager - Manages the active window controllers </li>
<li>uicore::WindowController - Controller for a window </li>
</ul>

<h2>Views</h2>
<ul>
<li>uicore::View - Base class for all views </li>
<li>uicore::ColumnView - View that defaults to column flex layout</li>
<li>uicore::RowView - View that defaults to row flex layout</li>
<li>uicore::ButtonBaseView - Unstyled clickable button</li>
<li>uicore::CheckBoxBaseView - Unstyled checkbox</li>
<li>uicore::ImageBaseView - Unstyled image</li>
<li>uicore::LabelBaseView - Unstyled text label</li>
<li>uicore::ProgressBaseView - Unstyled progress indicator </li>
<li>uicore::RadioButtonBaseView - Unstyled radio button</li>
<li>uicore::ScrollBaseView - Scrollable content with unstyled scrollbars</li>
<li>uicore::ScrollBarBaseView - Unstyled scrollbar</li>
<li>uicore::SliderBaseView - Unstyled slider</li>
<li>uicore::TextBlockBaseView - Unstyled rich text block</li>
<li>uicore::SpinBaseView - Unstyled spin control</li>
<li>uicore::TextFieldBaseView - Unstyled single line text input control</li>
<li>uicore::TextAreaBaseView - Unstyled multi line text input control </li>
</ul>

<h2>Style</h2>
<ul>
<li>uicore::Style - CSS styles specified for a view</li>
<li>uicore::StyleCascade - CSS computed value resolver</li>
</ul>

<h2>System Dialogs</h2>
<ul>
<li>uicore::BrowseFolderDialog - Displays the OS browse folder dialog</li>
<li>uicore::OpenFileDialog - Displays the OS open file dialog</li>
<li>uicore::SaveFileDialog - Displays the OS save file dialog</li>
</ul>

<h2>UI Thread</h2>
<ul>
<li>uicore::UIThread - Singleton for communicating with the UI thread</li>
</ul>

<h2>View Trees</h2>
<ul>
<li>uicore::TopLevelWindow - View tree presented in an OS top-level window </li>
<li>uicore::TextureWindow - View tree presented in a 2D texture </li>
<li>uicore::ViewTree - Base class for view trees</li>
</ul>

<h2>Image</h2>
<ul>
<li>uicore::ImageSource - Deferred image loading abstraction for image views</li>
</ul>

<h2>Events</h2>
<ul>
<li>uicore::EventUI - Base class for dispatched UI events </li>
<li>uicore::KeyEvent - Keyboard input events</li>
<li>uicore::PointerEvent - Pointer/mouse input events </li>
<li>uicore::FocusChangeEvent - View focus gained or lost notification</li>
<li>uicore::ActivationChangeEvent - Window focus gained or lost notification </li>
<li>uicore::ResizeEvent - View tree resize notification</li>
<li>uicore::CloseEvent - View tree close notification</li>
</ul>

*/
