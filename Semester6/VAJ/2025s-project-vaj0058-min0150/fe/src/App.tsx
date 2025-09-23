import { Outlet } from 'react-router-dom';
import Navigation from './components/Navigation';
import Footer from './components/Footer';

export default function App() {
    return (
        <main className="bg-background font-roboto text-text flex min-h-screen w-screen flex-col overflow-x-hidden text-lg lg:text-xl">
            <Navigation />
            <section className="flex flex-1 flex-col items-center">
                <Outlet />
            </section>
            <Footer />
        </main>
    );
}
