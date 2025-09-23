import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import { createBrowserRouter, RouterProvider } from 'react-router-dom';
import { paths } from './components/paths';
import App from './App.tsx';

import './index.css';
import 'bootstrap-icons/font/bootstrap-icons.min.css';
import './fonts.css';

const router = createBrowserRouter([
    {
        path: '/',
        element: <App />,
        children: paths.map((path) => {
            return {
                path: path.path,
                element: path.element,
                index: path.index
            };
        })
    }
]);

createRoot(document.getElementById('root')!).render(
    <StrictMode>
        <RouterProvider router={router} />
    </StrictMode>
);
