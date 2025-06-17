// ... existing code ...
                // Publisher Management Buttons
                case 401: { // Add Publisher
                    Publisher publisher = getPublisherFromInputs(hwnd);
                    if (!isEmpty(publisher.name)) {
                        addPublisher(publisher);
                        refreshPublisherList(hwnd);
                        clearPublisherInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please enter the publisher\'s name.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }
                case 402: { // Update Publisher
                    Publisher publisher = getPublisherFromInputs(hwnd);
                    if (publisher.publisher_id > 0 && !isEmpty(publisher.name)) {
                        updatePublisher(publisher);
                        refreshPublisherList(hwnd);
                        clearPublisherInputs(hwnd);
                    } else {
                        MessageBox(hwnd, "Please provide a valid Publisher ID and name for update.", "Input Error", MB_OK | MB_ICONERROR);
                    }
                    break;
                }
// ... existing code ...