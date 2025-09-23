import { BootstrapIcon } from '$/types/bootstrap_icons';
import Shop from '$/routes/Shop';
import Home from '$/routes/Home';
import About from '$/routes/About';

type Path = {
    index?: boolean;
    name: string;
    path: string;
    icon: BootstrapIcon;
    element: React.ReactNode;
};

export const paths = [
    {
        index: true,
        name: 'Home',
        path: '',
        icon: 'bi-house-door-fill',
        element: <Home />
    },
    {
        name: 'Shop',
        path: 'shop',
        icon: 'bi-basket-fill',
        element: <Shop />
    },
    {
        name: 'About',
        path: 'about',
        icon: 'bi-info-circle-fill',
        element: <About />
    }
] satisfies Path[];
